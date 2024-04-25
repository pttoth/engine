#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/name.h"
#include <vector>

// TODO: remove instance-based file loaders
//       replace with static generator functions (factory pattern)

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Texture2d )

class Texture2d
{
public:
    Texture2d();
    Texture2d( const pt::Name& name );

    Texture2d( const Texture2d& other ) = delete;
    Texture2d( Texture2d&& source );
    virtual ~Texture2d();
    Texture2d& operator=( const Texture2d& other ) = delete;
    Texture2d& operator=( Texture2d&& source );

    bool operator==( const Texture2d& other ) const = delete;

    static bool     Initialize();

    void            Bind();
    void            FreeClientsideData();
    void            FreeVRAM();
    const std::string& GetFullName() const;
    GLuint          GetHandle() const;
    uint32_t        GetHeight() const;
    pt::Name        GetName() const;
    std::string     GetPath() const;
    math::int2      GetResolution() const;
    inline size_t   GetVRAMBytes() const;
    uint32_t        GetWidth() const;
    bool            IsLoadedInRAM() const;
    bool            IsLoadedInVRAM() const;
    void            LoadToVRAM();
    void            ReadFilePNG( const std::string& path );
    void            ReadTextureData( const std::string& path,
                                     const math::int2& resolution,
                                     const std::vector<math::float4>& data );
    void            Unbind();

protected:
    static Texture2dPtr GetFallbackTexture();

    //TODO: check, whether fallback is initialized and kill off program with a "Texture is uninitialized" error
    //  this won't work in ctor as the static instance's ctor is run there too
    static Texture2dPtr stFallbackTexture;
private:
    inline void SetDefaultMemberValues(){
        mBytesVRAM  = 0;
        mHandle     = 0;
        mName       = pt::Name();
        mPath       = std::string();
        mResolution = math::int2();
        mData       = std::vector<math::float4>();
        mCacheFullName = std::string();
    }

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
