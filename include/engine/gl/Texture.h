#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/name.h"
#include <vector>

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

    void            Bind();
    void            FreeClientsideData();
    void            FreeVRAM();
    GLuint          GetHandle() const;
    uint32_t        GetHeight() const;
    pt::Name        GetName() const;
    std::string     GetPath() const;
    math::int2      GetResolution() const;
    inline size_t   GetVRAMBytes() const;
    uint32_t        GetWidth() const;
    bool            IsLoadedInRAM() const;
    bool            IsLoadedInVRAM() const;
    void            LoadToVRAM( BufferTarget target, BufferHint hint );
    void            ReadFilePNG( const std::string& path );

protected:
private:
    size_t      mBytesVRAM = 0;
    GLuint      mHandle = 0;
    pt::Name    mName;
    std::string mPath;
    math::int2  mResolution;
    std::vector<math::float4> mData;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
