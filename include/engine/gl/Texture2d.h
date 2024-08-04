#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/name.h"
#include <vector>

// TODO: update state model to handle filling texture VRAM from GPU-based source
//       implement 'DownloadFromVRAM()'

// @TODO: add new members + setter + getters
//          LoD level

// @TODO: add Mipmap control logic
// @TODO: add LoD control logic

// @TODO: test mipmap and filtering quality with interactive filtering switching

// TODO: retrofit for shadow mapping ...or create new, Shadow Map class with lots of overlap... :(
//   Done - make RGBA buffer switchable to Depth and other types
//   Done - add parameters to Bind()
//          document some functions a little better

// @TODO: add move-semantic version of CreateFromData()

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Texture2d )

class Texture2d
{
public:
    static const GLint  stDefaultParamInternalFormat  = GL_RGBA;
    static const GLenum stDefaultParamFormat          = GL_RGBA;
    static const GLenum stDefaultParamType            = GL_FLOAT;

    virtual ~Texture2d();

    Texture2d( const Texture2d& other ) = delete;
    Texture2d( Texture2d&& source ) = delete;
    Texture2d& operator=( const Texture2d& other ) = delete;
    Texture2d& operator=( Texture2d&& source ) = delete;

    bool operator==( const Texture2d& other ) const = delete;

    static Texture2dPtr CreateEmpty( const std::string& name, const math::int2 resolution, GLint internal_format = stDefaultParamInternalFormat, GLenum format = stDefaultParamFormat, GLenum type = stDefaultParamType );
    static Texture2dPtr CreateFromData( const std::string& name, const math::int2 resolution, const std::vector<float>& data, GLint internal_format = stDefaultParamInternalFormat, GLenum format = stDefaultParamFormat, GLenum type = stDefaultParamType );
    static Texture2dPtr CreateFromPNG( const std::string& name, const std::string& path );

    static bool         Initialize();   // generates fallback textures (...that can be queried with 'GetFallback...()')
    static Texture2dPtr GetFallbackTexture();
    static Texture2dPtr GetFallbackMaterialTexture();
    static void         Unbind();

    void            ApplyTextureParameters();
    void            BindToTextureUnit( uint32_t texture_unit );
    void            DownloadFromVRAM();     // @TODO: implement
    void            FreeClientsideData();
    void            FreeVRAM();
    GLint           GetDataInternalFormat() const;
    GLenum          GetDataFormat() const;
    const std::string GetFullName() const;
    GLuint          GetHandle() const;
    uint32_t        GetHeight() const;
    gl::MagFilter   GetMagFilter() const;
    gl::MinFilter   GetMinFilter() const;
    pt::Name        GetName() const;
    std::string     GetPath() const;
    math::int2      GetResolution() const;
    inline size_t   GetVRAMBytes() const;
    uint32_t        GetWidth() const;
    gl::WrapRule    GetWrapRule() const;
    bool            HasDataInRAM() const;
    bool            HasDataInVRAM() const;
    void            LoadToVRAM();
    void            SetMagFilter( gl::MagFilter rule );
    void            SetMinFilter( gl::MinFilter rule );
    void            SetWrapRule( gl::WrapRule rule );

protected:
    Texture2d();
    Texture2d( const std::string& name );

    static std::string  GenerateNameFromPath( const std::string& path );

    /**
     * @brief Calculates the size of one pixel's worth of data on the RAM side.
     */
    static uint8_t      GetFormatDataSize( GLenum format, GLenum type );
    static bool         IsFormatValid( GLint internal_format, GLenum format, GLenum type );
    void                UpdateTextureParams();

    //TODO: check, whether fallback is initialized and kill off program with a "Texture is uninitialized" error
    //  this won't work in ctor as the static instance's ctor is run there too
    static Texture2dPtr stFallbackTexture;
    static Texture2dPtr stFallbackMaterialTexture;

private:
    //note: Querying mipmap level count:
    //  implementations may not follow spec on queries!
    //    ( sauce: https://stackoverflow.com/questions/9572414/how-many-mipmaps-does-a-texture-have-in-opengl )
    //  standard formula: numLevels = 1 + floor(log2(max(w, h, d)))

    GLint           mParamInternalFormat    = stDefaultParamInternalFormat;     // data format (in VRAM)
    GLenum          mParamFormat            = stDefaultParamFormat;             // data format (in RAM)
    GLenum          mParamType              = stDefaultParamType;               // element size (in RAM)
    GLint           mParamLoD               = 0;  // @TODO: implement
    gl::MinFilter   mParamMinFilter = gl::MinFilter::LINEAR;
    gl::MagFilter   mParamMagFilter = gl::MagFilter::LINEAR;
    gl::WrapRule    mParamWrapS     = gl::WrapRule::REPEAT;
    gl::WrapRule    mParamWrapT     = gl::WrapRule::REPEAT;
    bool            mParamsDirty    = true;

    uint8_t     mDataSize   = 4;    // size of pixel data in bytes
    size_t      mBytesVRAM  = 0;
    GLuint      mHandle     = 0;
    std::string mName;
    std::string mPath;
    math::int2  mResolution;
    std::vector<float>          mData;

    mutable std::string mCacheFullName;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
