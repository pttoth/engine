#include "engine/gl/Texture.h"

#include "engine/gl/GlWrapper.h"
#include "pt/logging.h"
#include "pt/guard.hpp"

#include "libpng/png.h"
#include <stdio.h>

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


Texture2d::
Texture2d()
{}


Texture2d::
Texture2d( const pt::Name& name ):
    mName( name )
{
    PT_LOG_ONCE_WARN( "WARNING: Texture2d constructor is unfinished!" );
}


Texture2d::
Texture2d( Texture2d&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


Texture2d::
~Texture2d()
{
    FreeVRAM();
    FreeClientsideData();
}


Texture2d& Texture2d::
operator=( Texture2d&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
    return *this;
}


void Texture2d::
Bind()
{
    if( 0 == mHandle ){
        PT_LOG_ERR( "Tried to bind texture '" << mPath << "' with 0 as handle" );
    }
    gl::BindTexture( GL_TEXTURE_2D, mHandle );
}


void Texture2d::
FreeClientsideData()
{
    mData = std::vector<math::float4>(); // .clear() doesn't release memory
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


pt::Name Texture2d::
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


bool Texture2d::
IsLoadedInRAM() const
{
    return ( 0 != mData.size() );
}


bool Texture2d::
IsLoadedInVRAM() const
{
    return ( 0 != mBytesVRAM );
}


void Texture2d::
LoadToVRAM( const BufferTarget target, const BufferHint hint )
{
    PT_LOG_DEBUG( "Loading texture '" << mPath << "' to GPU." );
    if( 0 == mHandle ){
        gl::GenTextures( 1, &mHandle );
        assert( 0 != mHandle );
        if( 0 == mHandle ){
            PT_LOG_ERR( "Failed to generate texture on GPU." );
            return;
        }
    }

    gl::BindTexture( GL_TEXTURE_2D, mHandle );
    gl::TexImage2D( GL_TEXTURE_2D,
                   /*LOD*/ 0, GL_RGBA,
                   mResolution.x, mResolution.y,
                   /*border*/ 0, GL_RGBA,
                   GL_FLOAT,
                   mData.data() );

    GLenum  errorcode = gl::GetError();
    if( GL_NO_ERROR != errorcode ){
        PT_LOG_ERR( "Failed to load texture '" << mPath << "' to VRAM"
                    << "'\n  (" << gl::GetErrorString( errorcode )
                    << "):\n" << gl::GetErrorDescription( errorcode ) );
        return;
    }

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    mBytesVRAM = mData.size() * sizeof( math::float4 );
}


void Texture2d::
ReadFilePNG( const std::string& path )
{
    ImageDataPNG imageData = PNG_ReadFile( path );
    if( imageData.IsEmpty() ){
        PT_LOG_ERR( "Failed to read PNG file '" << path << "'." );
        return;
    }
    auto imageDataGuard = pt::CreateGuard( [&imageData]{
        imageData.Free();
    } );

    FreeVRAM();
    FreeClientsideData();

    size_t size = imageData.height * imageData.width;
    mResolution = math::int2( imageData.width, imageData.height );
    mData.resize( size );

    int32_t w = mResolution.x;
    int32_t h = mResolution.y;
    for( int32_t j=0; j<h; ++j){
        for( int32_t i=0; i<w; ++i){
            mData[j*w + i] = math::float4( imageData.row_pointers[j][i*4+0] / 255.0f,
                                           imageData.row_pointers[j][i*4+1] / 255.0f,
                                           imageData.row_pointers[j][i*4+2] / 255.0f,
                                           imageData.row_pointers[j][i*4+3] / 255.0f );
        }
    }

    mPath = path;
    PT_LOG_OUT( "Loaded PNG file '" << mPath << "' to texture '" << mName.GetStdString() << "'." );
}
