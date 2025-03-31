#include "engine/gl/Texture2d.h"

#include "engine/gl/Buffer.hpp"
#include "engine/gl/GlWrapper.h"
#include "engine/Services.h"
#include "engine/service/Renderer.h"
#include "pt/logging.h"
#include "pt/guard.hpp"

#include "libpng/png.h"
#include <algorithm>
#include <assert.h>
#include <stdio.h>

uint32_t    engine::gl::Texture2d::stTextureMaxSize = 0;
uint32_t    engine::gl::Texture2d::stDummyHandle    = 0;

using namespace math;

struct ImageDataPNG
{
    png_structp data = nullptr;
    png_infop   info = nullptr;
    png_infop   endinfo = nullptr;
    png_byte    color_type = 0;
    png_byte    bit_depth = 0;
    size_t      width = 0;
    size_t      height = 0;
    png_bytep*  row_pointers = nullptr;


    ImageDataPNG() = default;
    ImageDataPNG( const ImageDataPNG& other ) = default;
    ImageDataPNG& operator=( const ImageDataPNG& other ) = default;

    void Free(){
        PT_LOG_DEBUG( "Freeing up temporary PNG data." );
        png_destroy_read_struct( &data, &info, &endinfo );
        for( size_t j=0; j<height; ++j ){
            free( row_pointers[j] );
        }
        free( row_pointers );
        *this = ImageDataPNG(); // re-initialize as empty
    }

    bool IsEmpty() const{
        return (data == nullptr)
            && (info == nullptr)
            && (endinfo == nullptr);
    }
};

/**
 * @brief PNG_ReadFile
 * Reads the PNG file named by 'filename'.
 * Important: libpng's "magic" causes it to exit with a jump to right after the 'PNG_ReadFile' invocation.
 *   If 'success' is false after the call to 'PNG_ReadFile', call free() on 'row_pointers' variable on the caller side!
 */
ImageDataPNG
PNG_ReadFile( const std::string& filename )
{
    // check whether file exists and is readable
    FILE* fptr = fopen( filename.c_str(), "rb" );
    if ( nullptr == fptr ){
        PT_LOG_ERR( "Could not open file '" << filename << "'." );
        return ImageDataPNG();
    }
    auto fileGuard = pt::CreateGuard( [fptr, &filename]{
        PT_LOG_DEBUG( "Closing file '" << filename << "'" );
        fclose( fptr );
    });

    // check whether file is a PNG image file
    unsigned char sig[8];
    fread( sig, 1, 8, fptr );
    if ( !png_check_sig( sig, 8 ) ){
        PT_LOG_ERR( "Tried to parse PNG data from file '" << filename << "', that is not a PNG file!" );
        return ImageDataPNG();
    }

    //create meta structures and memory guards
    ImageDataPNG pngData;
    auto pngStructGuard = pt::CreateGuard( [&pngData]{
        pngData.Free();
    } );
    pngData.data = png_create_read_struct( PNG_LIBPNG_VER_STRING, nullptr, NULL, NULL );
    pngData.info = png_create_info_struct( pngData.data );
    if( (nullptr == pngData.data ) || ( nullptr == pngData.info ) ){
        PT_LOG_ERR( "Out of memory!" );
        return ImageDataPNG();
    }

    // Set up error handling logic to exit this function if errors happen.
    // setjmp: returns 0 when called (avoids 'if' body)
    //         returns 1 when jumped to (by 'longjmp'), and enters the 'if' body
    if( setjmp( png_jmpbuf( pngData.data) ) ){
        // memory safety is already handled by the guard object
        PT_LOG_ERR( "Error while reading PNG file '" << filename << "'" );
        return ImageDataPNG();
    }

    // read PNG data
    png_init_io( pngData.data, fptr );
    png_set_sig_bytes( pngData.data, 8 ); // msg for reader, that the header has already been read
    png_read_info( pngData.data, pngData.info );

    pngData.width      = png_get_image_width( pngData.data, pngData.info );
    pngData.height     = png_get_image_height( pngData.data, pngData.info );
    pngData.color_type = png_get_color_type( pngData.data, pngData.info );
    pngData.bit_depth  = png_get_bit_depth( pngData.data, pngData.info );

    if( pngData.bit_depth == 16 ){
        png_set_strip_16( pngData.data );
    }
    if( pngData.color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_palette_to_rgb( pngData.data );
    }

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if( pngData.color_type == PNG_COLOR_TYPE_GRAY && ( pngData.bit_depth ) < 8){
        png_set_expand_gray_1_2_4_to_8( pngData.data );
    }
    if( png_get_valid( pngData.data, pngData.info, PNG_INFO_tRNS ) ){
        png_set_tRNS_to_alpha( pngData.data );
    }

    // These color_type don't have an alpha channel then fill it with 0xff.
    if( ( pngData.color_type ) == PNG_COLOR_TYPE_RGB
      || ( pngData.color_type ) == PNG_COLOR_TYPE_GRAY
      || ( pngData.color_type ) == PNG_COLOR_TYPE_PALETTE )
    {
        png_set_filler( pngData.data, 0xFF, PNG_FILLER_AFTER );
    }
    if( (pngData.color_type) == PNG_COLOR_TYPE_GRAY
      || (pngData.color_type) == PNG_COLOR_TYPE_GRAY_ALPHA )
    {
        png_set_gray_to_rgb( pngData.data );
    }

    png_read_update_info( pngData.data, pngData.info );

    size_t rowbyte_size = png_get_rowbytes( pngData.data, pngData.info );
    if( (pngData.width)*4 != rowbyte_size ){
        PT_LOG_ERR( "Invalid PNG image data in file '" << filename << "'!" );
        return ImageDataPNG();
    }

    size_t allocated_row_count = 0;
    pngData.row_pointers = static_cast<png_bytep*>( malloc( sizeof(png_bytep)*(pngData.height) ) );
    if( 0 == pngData.row_pointers ){
        return ImageDataPNG();
    }
    auto rowGuard = pt::CreateGuard( [&pngData, &allocated_row_count]{
        PT_LOG_ERR( "Out of memory!" );
        for( size_t j=0; j<allocated_row_count; ++j ){
            free( pngData.row_pointers[j] );
        }
        free( pngData.row_pointers );
    } );

    for( size_t j=0; j<pngData.height; ++j ){
        png_byte* currentptr = static_cast<png_byte*>( malloc( rowbyte_size ) );
        if( nullptr == currentptr ){ // if malloc failed, return and let 'rowGuard' free the memory
            return ImageDataPNG();
        }
        pngData.row_pointers[j] = currentptr;
        ++allocated_row_count;
    }

    png_read_image( pngData.data, pngData.row_pointers );

    // we want to close the file, so 'fileGuard' is not Disabled here, it'll close on return
    rowGuard.Disable();
    pngStructGuard.Disable();
    PT_LOG_DEBUG( "Successfully loaded PNG image '" << filename << "'." );
    return pngData;
}


using namespace engine::gl;
using namespace math;

Texture2d::
~Texture2d()
{
    FreeVRAM();
    FreeClientsideData();
}


Texture2dPtr Texture2d::
CreateEmpty( const std::string& name, math::int2 resolution, GLint internal_format, GLenum format, GLenum type )
{
    assert( 0 != name.length() );

    int32_t maxdimsize = (int32_t) stTextureMaxSize;

    const math::int2& res = resolution;
    assert( (0 < res.x) && (0 < res.y) );
    int32_t width   = res.x;
    int32_t height  = res.y;

    assert( 0 < maxdimsize );
    if( 0 < maxdimsize ){
        width   = pt::Clamp( 1, width, maxdimsize );
        height  = pt::Clamp( 1, height, maxdimsize );

        if( (width != res.x) || (height != res.y) ){
            PT_LOG_WARN( "Tried to create texture '" << name << "' with invalid resolution(" << res.x << ", " << res.y << ")!\n"
                         "    Using (" << width <<  ", " << height << ") instead." );
        }
    }

    WarnAboutMismatchingFormats( internal_format, format, type );

    Texture2dPtr instance = Texture2dPtr( new Texture2d( name ) );
    instance->mParamInternalFormat  = internal_format;
    instance->mParamFormat          = format;
    instance->mParamType            = type;
    instance->mResolution           = int2( width, height );

    PT_LOG_DEBUG( "Created texture '" << name << "'" );

    return instance;
}


/*
Texture2dPtr Texture2d::
CreateFromData( const std::string& name, math::int2 resolution, const std::vector<float>& data, GLint internal_format, GLenum format, GLenum type )
{
    PT_UNIMPLEMENTED_FUNCTION
    Texture2dPtr instance = CreateEmpty( name, resolution, internal_format, format, type );

    const math::int2& res = resolution;

    // reqCount: how many floats the buffer is made of
    // reqSize:  how many bytes  the buffer is made of
    const size_t reqCount   = res.x * res.y * Texture2d::GetFormatDataElemCount( format );
    const size_t reqSize    = reqCount * Texture2d::GetFormatDataSize( format, type );

    //...

    PT_LOG_DEBUG( "Loaded data to texture '" << name << "'." );

    return instance;
}
*/


Texture2dPtr Texture2d::
CreateFromData_RGBA_FLOAT( const std::string& name, const math::int2 resolution, const std::vector<float>& data )
{
    Texture2dPtr instance = CreateEmpty( name, resolution, GL_RGBA, GL_RGBA, GL_FLOAT );

    const math::int2& res = instance->GetResolution();

    // reqCount: how many floats the buffer is made of
    // reqSize:  how many bytes  the buffer is made of
    uint32_t reqCount   = res.x * res.y * 4;            // GL_RGBA  -> 4 floats per pixel
    uint32_t reqSize    = reqCount * sizeof(float);
    uint32_t dataCount  = data.size();
    uint32_t dataSize   = dataCount * sizeof(float);

    // failsafe padded buffer in case the provided data is less than size
    std::vector<float> failsafe_data;
    bool size_valid = (reqCount == dataCount);
    if( !size_valid ){
        PT_LOG_ERR( "Required amount of data don't match supplied data when creating texture'" << name << "' (size: " << dataSize << " bytes, required: " << reqSize << " bytes)!" );

        uint32_t usefulCount = std::min( reqCount, dataCount );
        failsafe_data.resize( reqCount );

        // fill available useful data into buffer
        for( size_t i=0; i<usefulCount; ++i ){
            failsafe_data[i] = data[i];
        }

        // add zero-padding to buffer, if needed
        for( size_t i=usefulCount; i<reqCount; ++i ){
            failsafe_data[i] = 0;
        }
    }

    instance->mData = (size_valid) ? data : std::move(failsafe_data);

    PT_LOG_DEBUG( "Loaded data (" << reqCount << " bytes) to texture '" << name << "'." );

    return instance;
}


Texture2dPtr Texture2d::
CreateFromPNG( const std::string& name, const std::string& path )
{
    ImageDataPNG imageData = PNG_ReadFile( path );
    if( imageData.IsEmpty() ){
        PT_LOG_ERR( "Failed to read PNG file '" << path << "' into texture '" << name << "'!" );
        return Texture2d::CreateStubTexture( name );
    }
    auto guard = pt::CreateGuard( [&imageData]{
        imageData.Free();
    } );

    int2 res = int2( imageData.width, imageData.height );
    if( (0 == res.x) && (0 == res.y) ){
        PT_LOG_ERR( "Invalid zero as texture size while reading texture '" << name << "' from PNG file '" << path << "'!" );
        return Texture2d::CreateStubTexture( name );
    }

    GLenum format   = GL_RGBA;
    GLenum type     = GL_FLOAT;
    Texture2dPtr instance = CreateEmpty( name, res, GL_RGBA, format, type );

    // res_actual: clamped image size [1, stTextureMaxSize]
    int2 res_actual = instance->GetResolution();

    instance->mPath = path;

    size_t dataSize = res_actual.y * res_actual.x * GetFormatDataElemCount( format );
    instance->mData.resize( dataSize );
    // if clamp happened, the image will get cropped
    int32_t w = res_actual.x;
    int32_t h = res_actual.y;
    for( int32_t j=0; j<h; ++j){
        for( int32_t i=0; i<w; ++i){
            // PNG coordinate system is "(0,0) - topleft"
            // The loader flips the image here, because OpenGL uses "(0,0) - bottomleft" coordinates
            //uint32_t idx = 4*(j*w + i);     // normal
            uint32_t idx = 4*((h-j-1)*w + i); // flipped
            instance->mData[idx+0] = imageData.row_pointers[j][i*4+0] / 255.0f;
            instance->mData[idx+1] = imageData.row_pointers[j][i*4+1] / 255.0f;
            instance->mData[idx+2] = imageData.row_pointers[j][i*4+2] / 255.0f;
            instance->mData[idx+3] = imageData.row_pointers[j][i*4+3] / 255.0f;
        }
    }

    PT_LOG_OUT( "Loaded PNG file '" << path << "' (" << dataSize << " bytes) to texture '" << name << "'." );

    return instance;
}


Texture2dPtr Texture2d::
CreateStubTexture( const std::string& name, math::int2 resolution )
{
    const math::int2& res = resolution;
    Texture2dPtr instance = CreateFromData_RGBA_FLOAT( name, res,
                                                       GenerateColorGrid( res.x, res.y,
                                                                          vec4(vec3::purple, 1.0f),
                                                                          vec4(vec3::black, 1.0f)   ) );
    instance->mIsStub = true;
    return instance;
}


std::vector<float> Texture2d::
GenerateColorGrid( uint32_t width, uint32_t height, math::vec4 color1, math::vec4 color2 )
{
    std::vector<float> data;
    data.resize( width * height * 4 );

    for( size_t j=0; j<height; ++j ){
        for( size_t i=0; i<width; ++i ){
            size_t idx = (j*width + i) *4;
            if( 0 == (i+j)%2 ){
                data[idx+0] = color1.r;
                data[idx+1] = color1.g;
                data[idx+2] = color1.b;
                data[idx+3] = color1.a;
            }else{
                data[idx+0] = color2.r;
                data[idx+1] = color2.g;
                data[idx+2] = color2.b;
                data[idx+3] = color2.a;
            }
        }
    }

    return data;
}


bool Texture2d::
Initialize( uint32_t texture_max_size )
{
    bool success = true;

    // save parameters
    stTextureMaxSize = texture_max_size;

    // create dummy texture
    PT_LOG_DEBUG( "Loading dummy texture to GPU" );
    if( 0 == stDummyHandle ){
        gl::GenTextures( 1, &stDummyHandle );
        if( 0 == stDummyHandle ){
            PT_LOG_ERR( "Failed to generate dummy texture on GPU." );
            success = false;
        }else{
            std::vector<float> data = GenerateColorGrid( 1,1,
                                                         vec4( vec3::black, 1.0f ),
                                                         vec4( vec3::black, 1.0f ) );

            gl::BindTexture( GL_TEXTURE_2D, stDummyHandle );
            gl::TexImage2D( GL_TEXTURE_2D,
                           0,
                           stDefaultParamInternalFormat,
                           1,1,
                           /*border*/ 0,
                           stDefaultParamFormat,
                           stDefaultParamType,
                           data.data() );

            GLenum  errorcode = gl::GetError();
            if( GL_NO_ERROR != errorcode ){
                PT_LOG_ERR( "Failed to load dummy texture to VRAM"
                            << "'\n  (" << gl::GetErrorString( errorcode )
                            << "):\n" << gl::GetErrorDescription( errorcode ) );
            }
        }
    }

    return success;
}


void Texture2d::
Deinitialize()
{
    if( 0 != stDummyHandle ){
        gl::DeleteTextures( 1, &stDummyHandle );
        stDummyHandle = 0;
    }
}


std::vector<Texture2dPtr> Texture2d::
GenerateUnicolorTextures()
{

    std::vector<Texture2dPtr>   textures;

    struct ColorEntry{
        vec3        mColor;
        std::string mName;
        ColorEntry( vec3 color, const char* name ):
            mColor( color ), mName( name )
        {}
    };

    std::vector<ColorEntry>     colors;
    colors.reserve( 32 );

    colors.push_back( ColorEntry( vec3::white,      "White" ) );
    colors.push_back( ColorEntry( vec3::red,        "Red" ) );
    colors.push_back( ColorEntry( vec3::green,      "Green" ) );
    colors.push_back( ColorEntry( vec3::blue,       "Blue" ) );
    colors.push_back( ColorEntry( vec3::cyan,       "Cyan" ) );
    colors.push_back( ColorEntry( vec3::yellow,     "Yellow" ) );
    colors.push_back( ColorEntry( vec3::gray,       "Gray" ) );
    colors.push_back( ColorEntry( vec3::teal,       "Teal" ) );


    const uint32_t w = 1;
    const uint32_t h = 1;
    const size_t   data_size = w * h * 4;
    std::vector<float> data;
    data.resize( data_size );

    textures.reserve( colors.size() );

    for( auto ce : colors ){
        std::string texname = "Unicolor" + ce.mName;

        for( size_t j=0; j<h; ++j ){        // create pixel data
            for( size_t i=0; i<w; ++i ){
                size_t idx = (j*w + i) *4;
                vec4 color = vec4( ce.mColor, 1.0f );
                data[idx+0] = color.r;
                data[idx+1] = color.g;
                data[idx+2] = color.b;
                data[idx+3] = color.a;
            }
        }

        gl::Texture2dPtr    tex = gl::Texture2d::CreateFromData_RGBA_FLOAT( texname, int2(w,h), data );
        tex->SetMinFilter( MinFilter::NEAREST );
        tex->SetMagFilter( MagFilter::NEAREST );
        textures.push_back( tex );

    }

    return textures;
}


void Texture2d::
Unbind()
{
    gl::BindTexture( GL_TEXTURE_2D, 0 );
}


void Texture2d::
ApplyTextureParameters()
{
    UpdateTextureParams();
}


void Texture2d::
BindToTextureUnit( uint32_t texture_unit )
{
    gl::ActiveTexture( texture_unit );
    GLuint handle = mHandle;

    if( !HasDataInVRAM() ){
        PT_LOG_LIMITED_ERR( 50, "Tried to bind texture " << GetFullName() << " without it being loaded in VRAM!" );
        assert( false );
        handle = stDummyHandle;
    }

    gl::BindTexture( GL_TEXTURE_2D, handle );

    // @TODO: delete these
    //mParamMinFilter = GL_LINEAR;                    // bilinear (slower)
    //mParamMinFilter = GL_NEAREST;
    //mParamMinFilter = GL_NEAREST_MIPMAP_NEAREST;
    //mParamMinFilter = GL_NEAREST_MIPMAP_LINEAR;
    //mParamMinFilter = GL_LINEAR_MIPMAP_NEAREST;     // bilinear /w mipmaps (faster)
    //mParamMinFilter = GL_LINEAR_MIPMAP_LINEAR;      // trilinear
    //-----

    UpdateTextureParams();
}


void Texture2d::
DownloadFromVRAM()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void Texture2d::
FreeClientsideData()
{
    mData = std::vector<float>();
}


void Texture2d::
FreeVRAM()
{
    if( 0 != mHandle ){
        gl::DeleteTextures( 1, &mHandle );
    }
    mHandle = 0;
    mBytesVRAM = 0;
}


GLint Texture2d::
GetDataInternalFormat() const
{
    return mParamInternalFormat;
}


GLenum Texture2d::
GetDataFormat() const
{
    return mParamFormat;
}


const std::string Texture2d::
GetFullName() const
{
    if( 0 == mCacheFullName.length() ){
        mCacheFullName.reserve( 1 + mName.length() + 9 + mPath.length() + 2 + 1 );
        mCacheFullName += "'" + mName + "'(path: '" + mPath + "')";
    }
    return mCacheFullName;
}


GLuint Texture2d::
GetHandle() const
{
    return mHandle;
}


uint32_t Texture2d::
GetHeight() const
{
    return mResolution.x;
}


MagFilter Texture2d::
GetMagFilter() const
{
    return mParamMagFilter;
}


MinFilter Texture2d::
GetMinFilter() const
{
    return mParamMinFilter;
}


std::string Texture2d::
GetName() const
{
    return mName;
}


std::string Texture2d::
GetPath() const
{
    return mPath;
}


math::int2 Texture2d::
GetResolution() const
{
    return mResolution;
}


size_t Texture2d::
GetVRAMBytes() const
{
    return mBytesVRAM;
}


uint32_t Texture2d::
GetWidth() const
{
    return mResolution.y;
}


WrapRule Texture2d::
GetWrapRule() const
{
    assert( mParamWrapS == mParamWrapT );
    return mParamWrapS;
}


bool Texture2d::
HasDataInRAM() const
{
    return ( 0 != mData.size() );
}


bool Texture2d::
HasDataInVRAM() const
{
    return ( 0 != mBytesVRAM );
}


bool Texture2d::
IsStub() const
{
    return mIsStub;
}


void Texture2d::
LoadToVRAM()
{
    if( !HasDataInRAM() ){
        PT_LOG_ERR( "Tried to load empty texture " << GetFullName() << " to VRAM. Skipping." );
        return;
    }

    PT_LOG_DEBUG( "Loading texture " << GetFullName() << " to GPU." );
    if( 0 == mHandle ){
        gl::GenTextures( 1, &mHandle );
        if( 0 == mHandle ){
            PT_LOG_ERR( "Failed to generate texture " << GetFullName() << " on GPU." );
            return;
        }
        mParamsDirty = true;
    }

    gl::BindTexture( GL_TEXTURE_2D, mHandle );
    gl::TexImage2D( GL_TEXTURE_2D,
                   mParamLoD,
                   mParamInternalFormat,
                   mResolution.x, mResolution.y,
                   /*border*/ 0,
                   mParamFormat,
                   mParamType,
                   mData.data() );

    GLenum  errorcode = gl::GetError();
    if( GL_NO_ERROR != errorcode ){
        PT_LOG_ERR( "Failed to load texture " << GetFullName() << " to VRAM"
                    << "'\n  (" << gl::GetErrorString( errorcode )
                    << "):\n" << gl::GetErrorDescription( errorcode ) );
        return;
    }

    //gl::GenerateMipmap( GL_TEXTURE_2D );

    UpdateTextureParams();

    mBytesVRAM = mData.size() * mDataSize;
}


void Texture2d::
SetMagFilter( MagFilter rule )
{
    mParamMagFilter = rule;
    mParamsDirty = true;
}


void Texture2d::
SetMinFilter( MinFilter rule )
{
    mParamMinFilter = rule;
    mParamsDirty = true;
}


void Texture2d::
SetWrapRule( WrapRule rule )
{
    mParamWrapS = rule;
    mParamWrapT = rule;
    mParamsDirty = true;
}


Texture2d::
Texture2d()
{}


Texture2d::
Texture2d( const std::string& name ):
    mName( name )
{}


std::string Texture2d::
GenerateNameFromPath( const std::string& path )
{
    // @TODO: implement
    return path;
}


uint8_t Texture2d::
GetFormatDataElemCount( GLenum format )
{
    uint8_t elementCount   = 4;

    // @TODO: finish...
    // Function is not yet implemented for anything that is not currently in use!
    switch( format ){
        case GL_RGBA:
            elementCount = 4;
            break;
        case GL_RGBA8:
            elementCount = 4;
            break;
        case GL_DEPTH_COMPONENT:
            elementCount = 1;
            break;
        default:
            PT_LOG_ERR( "Texture2d::GetFormatDataElemCount(): unknown format enum '" << format << "'" );
            PT_UNIMPLEMENTED_FUNCTION
            break;
    }

    return elementCount;
}


uint8_t Texture2d::
GetFormatDataSize( GLenum format, GLenum type )
{
    uint8_t elementSize    = 1;    // GL_FLOAT
    uint8_t elementCount   = GetFormatDataElemCount( format );

    // @TODO: finish...
    // Function is not yet implemented for anything that is not currently in use!
    if( GL_FLOAT != type ){
        PT_LOG_ERR( "Texture2d::GetFormatDataSize(): unknown type enum '" << type << "'" );
        PT_UNIMPLEMENTED_FUNCTION
    }

    return elementCount * elementSize;
}


bool Texture2d::
FormatsMatch( GLint internal_format, GLenum format, GLenum type )
{
    // barely handles any cases, rewrite as needed!
    // @TODO: finish implementation

    if( GL_FLOAT != type ){
        return false;
    }

    if( (GL_RGBA == internal_format) && (GL_RGBA == format) ){
        return true;
    }

    if( (GL_DEPTH_COMPONENT == internal_format) && (GL_DEPTH_COMPONENT == format) ){
        return true;
    }

    return false;
}


void Texture2d::
WarnAboutMismatchingFormats( GLint internal_format, GLenum format, GLenum type )
{
    if( not FormatsMatch( internal_format, format, type ) ){
        PT_LOG_WARN( "Texture format mismatch (internal_format: '" << internal_format << "', format '" << format << "', type '" << type << "');" );
    }
}


void Texture2d::
UpdateTextureParams()
{
    if( (mParamsDirty) && (0 < mHandle) ){
        PT_LOG_DEBUG( "Loading texture params for " << GetFullName() << " to GPU." );
        gl::TexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGLint( mParamMinFilter ) );
        gl::TexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGLint( mParamMagFilter ) );
        gl::TexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapRuleToGLint( mParamWrapS ) );
        gl::TexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapRuleToGLint( mParamWrapT ) );

        mParamsDirty = false;
    }
}
