#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/name.h"
#include <vector>

// TODO: remove instance-based file loaders
//       replace with static generator functions (factory pattern)

// TODO: update state model to allow filling texture VRAM from GPU-based source

// @TODO: add factory functions
// @TODO: remove public ctors
// @TODO: add new members + setter + getters
//          LoD level
//          MinMagFilter
//          WrapRule
//          TextureFormat + InternalFormat

// @TODO: add Mipmap enable logic

// @TODO: test mipmap and filtering quality with interactive filtering switching

// TODO: retrofit for shadow mapping ...or create new, Shadow Map class with lots of overlap... :(
//          make RGBA buffer switchable to Depth and other types
//          add parameters to Bind()
//          document some functions a little better
//          move file reading to static factory functions
//            texture class "type" contents cannot be switched, they are assigned by factory and stay that way until destruction


namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Texture2d )

class Texture2d
{
public:
    Texture2d();
    Texture2d( const pt::Name& name );

    Texture2d( const Texture2d& other ) = delete;
    Texture2d( Texture2d&& source ) = delete;
    virtual ~Texture2d();
    Texture2d& operator=( const Texture2d& other ) = delete;
    Texture2d& operator=( Texture2d&& source ) = delete;

    bool operator==( const Texture2d& other ) const = delete;

    static bool         Initialize();   // generates fallback textures (...that can be queried with 'GetFallback...()')
    static Texture2dPtr GetFallbackTexture();
    static Texture2dPtr GetFallbackMaterialTexture();
    static void         Unbind();

    void            ApplyTextureParameters();
    void            BindToTextureUnit( uint32_t texture_unit );
    void            FreeClientsideData();
    void            FreeVRAM();
    const std::string& GetFullName() const;
    GLuint          GetHandle() const;
    uint32_t        GetHeight() const;
    gl::MinFilter   SetMinFilter() const;
    gl::MagFilter   SetMagFilter() const;
    pt::Name        GetName() const;
    std::string     GetPath() const;
    math::int2      GetResolution() const;
    inline size_t   GetVRAMBytes() const;
    uint32_t        GetWidth() const;
    gl::WrapRule    GetWrapRule() const;
    bool            IsLoadedInRAM() const;
    bool            IsLoadedInVRAM() const;
    void            LoadToVRAM();
    void            ReadFilePNG( const std::string& path );
    void            ReadTextureData( const std::string& path,
                                     const math::int2& resolution,
                                     const std::vector<math::float4>& data );

    void            SetMinFilter( gl::MinFilter filter );
    void            SetMagFilter( gl::MagFilter filter );
    void            SetWrapRule( gl::WrapRule rule );

protected:
    void            UpdateTextureParams();

    //TODO: check, whether fallback is initialized and kill off program with a "Texture is uninitialized" error
    //  this won't work in ctor as the static instance's ctor is run there too
    static Texture2dPtr stFallbackTexture;
    static Texture2dPtr stFallbackMaterialTexture;
private:
    //note: Querying mipmap level count:
    //  implementations may not follow spec on queries!
    //    ( sauce: https://stackoverflow.com/questions/9572414/how-many-mipmaps-does-a-texture-have-in-opengl )
    //  standard formula: numLevels = 1 + floor(log2(max(w, h, d)))

    //GLint           mParamMinFilter = GL_LINEAR_MIPMAP_LINEAR;     // trilinear (doesn't work if mipmapping is disabled!)
    GLint           mParamMinFilter = GL_LINEAR;                   // bilinear
    GLint           mParamMagFilter = GL_LINEAR;
    gl::WrapRule    mParamWrapS     = gl::WrapRule::REPEAT;
    gl::WrapRule    mParamWrapT     = gl::WrapRule::REPEAT;
    bool            mParamsDirty    = true;

    size_t      mBytesVRAM = 0;
    GLuint      mHandle = 0;
    pt::Name    mName;
    std::string mPath;
    math::int2  mResolution;
    std::vector<math::float4> mData;

    mutable std::string mCacheFullName;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
