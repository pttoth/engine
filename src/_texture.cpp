
#include "engine/texture.h"
#include "engine/drawhelpers.h"

#include "libpng/png.h"
#include "SDL2/SDL.h"

#include <cstdlib>
#include <assert.h>

#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

using namespace pt;
using namespace pt::engine;


size_t Texture::s_defresolution = 128;

void Texture::
_loadToRAMFromFile(const std::string &filename){
    ImageType imgtype = _identifyImageType(filename);
    bool success = false;
    switch(imgtype){
        case IMG_UNKNOWN:
            assert(false);
        break;
        case IMG_PNG:
            success = _importPNG(filename);
        break;
        case IMG_JPG:
        break;
        case IMG_BMP:
        break;
        case IMG_TGA:
        break;
    }


    //TODO: move this to a texture wrangler later
    //          if the tex isn't loaded, it returns a default tex
    //          this will avoid multiple black-purp tex allocations
    if ( !success ) {
        std::cout << "Texture Error: could not open file '" << filename << "'\n";
        m_data.clear();
        m_dataf.clear();
        m_default = true;
        bool oddline = true, oddcolumn = true;
        m_dataf.resize( s_defresolution * s_defresolution );
        for(size_t j=0; j<s_defresolution; ++j){
            oddline = j%16 < 8;
            for(size_t i=0; i<s_defresolution; ++i){
                oddcolumn = i%16 < 8;
                if(oddline&&oddcolumn){         m_dataf[j*s_defresolution+i] = ColorFloat( color_purple );
                }else if(oddline&&!oddcolumn){  m_dataf[j*s_defresolution+i] = ColorFloat( color_black  );
                }else if(!oddline&&oddcolumn){  m_dataf[j*s_defresolution+i] = ColorFloat( color_black  );
                }else if(!oddline&&!oddcolumn){ m_dataf[j*s_defresolution+i] = ColorFloat( color_purple );
                }
            }
        }
        m_path.clear();
        m_height = s_defresolution;
        m_width = s_defresolution;
    }else{
        std::cout << "Loaded texture '" << filename << "'\n";
    }
}


#pragma GCC diagnostic ignored "-Wsign-conversion"
void Texture::
_loadToGPUFromRAM(){
    glGenTextures(1, &m_textureobj_id);
    gl::BindTexture(GL_TEXTURE_2D, m_textureobj_id);

    bool UseFloatInsteadOfBytes = true;
    if(UseFloatInsteadOfBytes){
        gl::TexImage2D(GL_TEXTURE_2D,
                       /*LOD*/ 0, GL_RGBA,
                       m_width, m_height,
                       /*border*/ 0, GL_RGBA,
                       GL_FLOAT,
                       m_dataf.data() );
    }else{
        gl::TexImage2D(GL_TEXTURE_2D,
                       /*LOD*/ 0, GL_RGBA,
                       m_width, m_height,
                       /*border*/ 0, GL_RGBA,
                       GL_UNSIGNED_BYTE,
                       m_data.data() );
    }
/*
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_initialized = true;
}
#pragma GCC diagnostic warning "-Wsign-conversion"


Texture::ImageType Texture::
_identifyImageType(const std::string& /*filename*/){
    //assumes that file exists!
        //caller has to make sure!
    return IMG_PNG;
}


/**
 * @brief PNG_ReadFile
 * Reads the PNG file named by 'filename'.
 * The reason for the pointer parameters of this function is because 'libpng' uses a goto jump
 * when encountering errors. It causes the code to jump back from the calling function
 * to the line right after it. This would cause aborted read-transactions to leak memory,
 * therefore temporary resources have to be known by the function calling PNG_ReadFile,
 * so that cleanup is possible after a goto exit from PNG_ReadFile.
 */
void
PNG_ReadFile(const std::string& filename, FILE** fptr,
             png_structp* png, png_infop* info,
             png_byte* color_type, png_byte* bit_depth,
             size_t* width, size_t* height,
             png_bytep** row_pointers,
             bool* success){
    *success = false;

    //check if file exists
    *fptr = fopen(filename.c_str(), "rb");
    if (*fptr == nullptr){   return;  }

    //create meta structures
    *png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, NULL, NULL);
    if(!(*png)){ return; }
    *info = png_create_info_struct(*png);
    if(!(*info)){ return; }

    //set up goto jump location (next line after caller function)
    if( setjmp(png_jmpbuf(*png)) ){ return; }
    png_init_io(*png, *fptr);
    png_read_info(*png, *info);

    *width      = png_get_image_width(*png, *info);
    *height     = png_get_image_height(*png, *info);
    *color_type = png_get_color_type(*png, *info);
    *bit_depth  = png_get_bit_depth(*png, *info);

    if(*bit_depth == 16){
        png_set_strip_16(*png);
    }
    if(*color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_palette_to_rgb(*png);
    }
    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(*color_type == PNG_COLOR_TYPE_GRAY && (*bit_depth) < 8){
        png_set_expand_gray_1_2_4_to_8(*png);
    }
    if(png_get_valid(*png, *info, PNG_INFO_tRNS)){
        png_set_tRNS_to_alpha(*png);
    }
    // These color_type don't have an alpha channel then fill it with 0xff.
    if((*color_type) == PNG_COLOR_TYPE_RGB
    || (*color_type) == PNG_COLOR_TYPE_GRAY
    || (*color_type) == PNG_COLOR_TYPE_PALETTE){
        png_set_filler(*png, 0xFF, PNG_FILLER_AFTER);
    }
    if((*color_type) == PNG_COLOR_TYPE_GRAY
    || (*color_type) == PNG_COLOR_TYPE_GRAY_ALPHA){
        png_set_gray_to_rgb(*png);
    }

    png_read_update_info(*png, *info);

    size_t rowbyte_size = png_get_rowbytes(*png,*info);
    if( (*width)*4 != rowbyte_size ){
        *success = false;
        return;
    }

    *row_pointers = static_cast<png_bytep*>( malloc(sizeof(png_bytep)*(*height) ) );
    for(size_t j=0; j<*height; ++j) {
        (*row_pointers)[j] = static_cast<png_byte*>( malloc(rowbyte_size) );
    }
    png_read_image(*png, *row_pointers);
//-----
    *success = true;
}

bool Texture::
_importPNG(const std::string &filename){
    FILE*           fptr            = nullptr;
    png_structp     png             = nullptr;
    png_infop       info            = nullptr;
    png_byte        color_type      = 0;
    png_byte        bit_depth       = 0;
    size_t          width           = 0;
    size_t          height          = 0;
    png_bytep*      row_pointers    = nullptr;
    bool            success         = false;


    PNG_ReadFile(filename,
                 &fptr, &png, &info,
                 &color_type, &bit_depth,
                 &width, &height,
                 &row_pointers,
                 &success);
    //important:
    //  PNG_ReadFile may jump here with a goto when encountering errors.
    //  Any resource allocated up to that point has to be freed before exiting!
    //  An failed read can be identified by the 'success' boolean.
    //  The allocations occured can be indentified by local pointers having values
    //    different from 'nullptr'.
    //  Note that PNG_ReadFile cannot write member variables, we don't need to worry
    //    about freeing those.


    //load data from function resources to class variables
    if( success ){
        size_t size = height * width;
        m_width     = width;
        m_height    = height;
        try{
            m_data.resize(size);
            m_dataf.resize(size);
            for(size_t j=0; j<m_height; ++j){
                for(size_t i=0; i<m_width; ++i){
                    m_data[j*m_width+i] = ColorByte(row_pointers[j][i*4+0],
                                                    row_pointers[j][i*4+1],
                                                    row_pointers[j][i*4+2],
                                                    row_pointers[j][i*4+3] );
                    m_dataf[j*m_width+i] = ColorFloat(row_pointers[j][i*4+0] / 255.0f,
                                                      row_pointers[j][i*4+1] / 255.0f,
                                                      row_pointers[j][i*4+2] / 255.0f,
                                                      row_pointers[j][i*4+3] / 255.0f );
                }
            }
        }catch(const std::bad_alloc&){
            m_data.clear();
            m_dataf.clear();
            success = false;
        }
        //dump function resources
        for(size_t j=0; j<m_height; ++j){
            free(row_pointers[j]);
        }
        free(row_pointers);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fptr);
    }
    return success;
}

Texture::Texture():
    Asset (),
    m_initialized(false),
    m_textureobj_id(static_cast<GLuint> (-1)),
    m_default(false),
    m_width(0),
    m_height(0)
{

}



Texture::
Texture(const TextureDescriptor &descriptor):
    Asset (descriptor),
    m_initialized(false),
    m_textureobj_id(static_cast<GLuint> (-1)),
    m_default(false),
    m_width(0),
    m_height(0)
{
}

Texture::
~Texture(){
    unload();
}

void Texture::
bind(GLenum TextureUnitID, GLenum target){
    gl::ActiveTexture(TextureUnitID);
    gl::BindTexture(target, m_textureobj_id);
}

void Texture::
loadToGPU(const size_t width, const size_t height, const uint8_t *data){
    unload();
    m_data.resize(width*height);
    m_dataf.resize(width*height);
    m_width  = width;
    m_height = height;

    for(size_t j=0; j<m_height; ++j){
        for(size_t i=0; i<m_width; ++i){
            m_data[j*m_width+i] = ColorByte(data[j*m_width*4 + i*4 +0],
                                            data[j*m_width*4 + i*4 +1],
                                            data[j*m_width*4 + i*4 +2],
                                            data[j*m_width*4 + i*4 +3] );
            m_dataf[j*m_width+i] = ColorFloat(  data[j*m_width*4 + i*4 +0] / 255.0f,
                                                data[j*m_width*4 + i*4 +1] / 255.0f,
                                                data[j*m_width*4 + i*4 +2] / 255.0f,
                                                data[j*m_width*4 + i*4 +3] / 255.0f );
        }
    }
    _loadToGPUFromRAM();
}

void Texture::
loadToGPUFromFile(const std::string &filename){
    unload();
    _loadToRAMFromFile(filename);
    _loadToGPUFromRAM();
}

void Texture::
unload(){
    m_data.clear();
    m_dataf.clear();
    if(isInitialized()){
        gl::DeleteTextures(1, &m_textureobj_id);
        m_textureobj_id = 0;
        m_width = 0;
        m_height = 0;
        m_path.clear();
    }
    m_initialized = false;
}
