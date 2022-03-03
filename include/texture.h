#pragma once

#include "engine/glhelpers.h"
//#include "engine/mathhelpers.hpp"
#include "engine/asset.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <string>
#include <vector>


namespace pt{
namespace engine{

struct TextureDescriptor: public AssetDescriptor{

}; //end of struct TextureDescriptor

class Texture: public Asset{
    struct ColorByte{
        uint8_t r,g,b,a;
        ColorByte(uint8_t red   = 0,
                  uint8_t green = 0,
                  uint8_t blue  = 0,
                  uint8_t alpha = 255): r(red),
                                        g(green),
                                        b(blue),
                                        a(alpha){
        }
    };

    struct ColorFloat{
        float r,g,b,a;
        ColorFloat( float red   = 0,
                    float green = 0,
                    float blue  = 0,
                    float alpha = 1.0f): r(red),
                                         g(green),
                                         b(blue),
                                         a(alpha){
        }
        ColorFloat( const math::float3& color,
                    float alpha = 1.0f ): r(color.x),
                                          g(color.y),
                                          b(color.z),
                                          a(alpha){
        }
    };

    enum ImageType{
        IMG_UNKNOWN = 0,
        IMG_PNG     = 1,
        IMG_JPG     = 2,
        IMG_BMP     = 3,
        IMG_TGA     = 4,
    };

    static size_t s_defresolution;

    //inner set variables
    bool                    m_initialized;
    GLuint                  m_textureobj_id;
    bool                    m_default;
    //foreign set variables
    std::string             m_path;
    size_t                  m_width, m_height;
    std::vector<ColorByte>  m_data;
    std::vector<ColorFloat> m_dataf;

    void        _loadToRAMFromFile( const std::string& filename );
    void        _loadToGPUFromRAM();
    ImageType   _identifyImageType(const std::string& filename); //todo: implement correctly

    bool        _importPNG(const std::string& filename);
public:
    Texture();
    Texture(const TextureDescriptor& descriptor);
    ~Texture();

    bool        isInitialized() const{ return m_initialized; }
    bool        isDefault()const{ return m_default; }
    size_t      getWidth() const{ return m_width; }
    size_t      getHeight() const{ return m_height; }

    //not const!
    GLuint      getTextureID() { return m_textureobj_id; }

    void        bind(GLenum TextureUnitID, GLenum target=GL_TEXTURE_2D);
    void        loadToGPU(const size_t width, const size_t height, const uint8_t* data);
    void        loadToGPUFromFile(const std::string& filename);
    void        unload();
};

} //end of namespace engine
} //end of namespace PT
