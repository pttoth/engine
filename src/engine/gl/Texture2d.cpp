#include "engine/gl/Texture2d.h"

#include "engine/gl/Buffer.hpp"
#include "engine/gl/GlWrapper.h"
#include "engine/Services.h"
#include "engine/service/DrawingControl.h"
#include "pt/logging.h"
#include "pt/guard.hpp"

#include "libpng/png.h"
#include <assert.h>
#include <stdio.h>

engine::gl::Texture2dPtr engine::gl::Texture2d::stFallbackTexture = nullptr;
engine::gl::Texture2dPtr engine::gl::Texture2d::stFallbackMaterialTexture = nullptr;

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

    pngData.row_pointers = static_cast<png_bytep*>( malloc( sizeof(png_bytep)*(pngData.height) ) );
    if( 0 == pngData.row_pointers ){
        PT_LOG_ERR( "Out of memory!" );
        return ImageDataPNG();
    }

    size_t actual_row_count = 0;
    for( size_t j=0; j<pngData.height; ++j ){
        png_byte* currentptr = static_cast<png_byte*>( malloc( rowbyte_size ) );
        if( nullptr == currentptr ){ // malloc failed
            break;
        }
        pngData.row_pointers[j] = currentptr;
        ++actual_row_count;
    }
    auto rowGuard = pt::CreateGuard( [&pngData, actual_row_count]{
        for( size_t j=0; j<actual_row_count; ++j ){
            free( pngData.row_pointers[j] );
        }
        free( pngData.row_pointers );
    } );

    if( actual_row_count != pngData.height ){
        PT_LOG_ERR( "Out of memory!" );
        return ImageDataPNG();
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
CreateEmpty( const std::string& name, const math::int2 resolution, GLint internal_format, GLenum format, GLenum type )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Tried to create texture with empty name." );
        return nullptr;
    }

    if( resolution.x < 1 || resolution.y < 1 ){
        PT_LOG_WARN( "Tried to create texture '" << name << "' with invalid resolution(" << ToString(resolution) << ")" );
        return nullptr;
    }

    if( !IsFormatValid( internal_format, format, stDefaultParamType ) ){
        return nullptr;
    }

    Texture2dPtr instance = Texture2dPtr( new Texture2d( name ) );
    instance->mParamInternalFormat  = internal_format;
    instance->mParamFormat          = format;
    instance->mParamType            = type;
    instance->mResolution           = resolution;

    PT_LOG_DEBUG( "Created texture '" << name << "'" );

    return instance;
}


Texture2dPtr Texture2d::
CreateFromData( const std::string& name, const math::int2 resolution, const std::vector<float>& data, GLint internal_format, GLenum format, GLenum type )
{
    const size_t reqSize = resolution.x * resolution.y * Texture2d::GetFormatDataSize( format, type );
    if( data.size() < reqSize ){
        PT_LOG_ERR( "Too small data buffer supplied when creating texture'" << name << "'! (size: " << data.size() << ", required: " << reqSize << ")" );
        return nullptr;
    }

    Texture2dPtr instance = CreateEmpty( name, resolution, internal_format, format, type );
    if( nullptr == instance ){
        return nullptr;
    }

    instance->mData = data;

    PT_LOG_DEBUG( "Loaded data to texture '" << name << "'" );

    return instance;
}


Texture2dPtr Texture2d::
CreateFromPNG( const std::string& name, const std::string& path )
{
    ImageDataPNG imageData = PNG_ReadFile( path );
    if( imageData.IsEmpty() ){
        PT_LOG_ERR( "Failed to read PNG file '" << path << "'." );
        return nullptr;
    }
    auto imageDataGuard = pt::CreateGuard( [&imageData]{
        imageData.Free();
    } );

    math::int2 resolution = math::int2( imageData.width, imageData.height );

    GLenum format   = GL_RGBA;
    GLenum type     = GL_FLOAT;
    Texture2dPtr instance = CreateEmpty( name,
                                         resolution, GL_RGBA, format, type );
    if( nullptr == instance ){
        return nullptr;
    }

    instance->mPath = path;
    instance->mData.resize( resolution.y * resolution.x * GetFormatDataSize( format, type ) );
    //PNG coordinate system is "(0,0) - topleft"
    //  The loader flips the image here, because OpenGL uses "(0,0) - bottomleft" coordinates
    int32_t w = resolution.x;
    int32_t h = resolution.y;
    for( int32_t j=0; j<h; ++j){
        for( int32_t i=0; i<w; ++i){
            //uint32_t idx = 4*(j*w + i);     // normal
            uint32_t idx = 4*((h-j-1)*w + i); // flipped
            instance->mData[idx+0] = imageData.row_pointers[j][i*4+0] / 255.0f;
            instance->mData[idx+1] = imageData.row_pointers[j][i*4+1] / 255.0f;
            instance->mData[idx+2] = imageData.row_pointers[j][i*4+2] / 255.0f;
            instance->mData[idx+3] = imageData.row_pointers[j][i*4+3] / 255.0f;
        }
    }

    PT_LOG_OUT( "Loaded PNG file '" << path << "' to texture '" << name << "'." );

    return instance;
}


bool Texture2d::
Initialize()
{
    if( (nullptr != stFallbackTexture) && (nullptr != stFallbackMaterialTexture) ){
        return true;
    }

    std::vector<std::string>        names;
    std::vector<vec4>               colors;
    std::vector<gl::Texture2dPtr*>  fallback_ptrs;    // where to save the generated texture

    names.push_back( "MissingTextureFallback" );
    colors.push_back( vec4( 1.0f, 0.0f, 1.0f, 1.0f ) ); // purple
    fallback_ptrs.push_back( &stFallbackTexture );

    names.push_back( "MissingMaterialFallback" );
    colors.push_back( vec4( 1.0f, 1.0f, 0.0f, 1.0f ) ); // yellow
    fallback_ptrs.push_back( &stFallbackMaterialTexture );
    assert( (names.size() == colors.size()) && (colors.size() == fallback_ptrs.size()) );


    const uint32_t w = 16;
    const uint32_t h = 16;
    const size_t   data_size = w * h * 4;
    std::vector<float> data;
    data.resize( data_size );

    for( size_t k=0; k<names.size(); ++k ){ // for every fallback
        data.clear();
        data.resize( data_size );   // paranoid failsafe
        for( size_t j=0; j<h; ++j ){        // create a checkered grid /w two colors
            for( size_t i=0; i<w; ++i ){
                size_t idx = (j*w + i) *4;
                vec4 color;
                if( 0 == (i+j)%2 ){
                    color = colors[k];                  // grid color
                }else{
                    color = vec4( vec3::black, 1.0f );  // black
                }
                data[idx+0] = color.r;
                data[idx+1] = color.g;
                data[idx+2] = color.b;
                data[idx+3] = color.a;
            }
        }

        gl::Texture2dPtr    tex = gl::Texture2d::CreateFromData( names[k], int2(w,h), data,
                                                                 GL_RGBA, GL_RGBA, GL_FLOAT );
        *(fallback_ptrs[k]) = tex;
        tex->SetMinFilter( MinFilter::NEAREST );
        tex->SetMagFilter( MagFilter::NEAREST );
        tex->LoadToVRAM();
    }

    return true;
}


Texture2dPtr Texture2d::
GetFallbackTexture()
{
    return stFallbackTexture;
}


Texture2dPtr Texture2d::
GetFallbackMaterialTexture()
{
    return stFallbackMaterialTexture;
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
        handle = stFallbackTexture->GetHandle();
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
            PT_LOG_ERR( "Failed to generate memory for texture " << GetFullName() << " on GPU." );
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
GetFormatDataSize( GLenum format, GLenum type )
{
    uint8_t elementSize    = 1;    // GL_FLOAT
    uint8_t elementCount   = 4;    // GL_RGBA

    // @note: function is not yet implemented for anything that is not currently in use!

    if( GL_FLOAT != type ){
        PT_LOG_ERR( "Texture2d::GetFormatDataSize(): unknown type enum '" << type << "'" );
        PT_UNIMPLEMENTED_FUNCTION
    }

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
            PT_LOG_ERR( "Texture2d::GetFormatDataSize(): unknown format enum '" << format << "'" );
            PT_UNIMPLEMENTED_FUNCTION
            break;
    }

    return elementCount * elementSize;
}


bool Texture2d::
IsFormatValid( GLint internal_format, GLenum format, GLenum type )
{
    // VERY half-assed validation logic!
    // rewrite as needed

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
