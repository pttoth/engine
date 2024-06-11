#pragma once

#include "engine/gl/Def.h"

#include "engine/gl/Buffer.hpp"
#include "engine/gl/ShaderProgram.h"

#include "engine/Def.h"
#include "pt/macros.h"



namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( UniformBlockFrameInfo )


class UniformBlockFrameInfo
{
public:
    UniformBlockFrameInfo();
    virtual ~UniformBlockFrameInfo();
    UniformBlockFrameInfo( UniformBlockFrameInfo&& source );
    UniformBlockFrameInfo& operator=( UniformBlockFrameInfo&& source );

    UniformBlockFrameInfo( const UniformBlockFrameInfo& other ) = delete;
    UniformBlockFrameInfo& operator=( const UniformBlockFrameInfo& other ) = delete;
    bool operator==( const UniformBlockFrameInfo& other ) const = delete;

    void    BindBufferToBindingPoint( GLuint index );

    bool    IsClientSideSynced();
    void    FreeVRAM();
    GLuint  GetHandle() const;
    size_t  GetBytes() const;
    size_t  GetVRAMBytes() const;
    void    LoadToVRAM( BufferTarget target, BufferHint hint );

    void    SetT( float val );
    void    SetDT( float val );
    void    SetV( const math::mat4& mtx );
    void    SetVrot( const math::mat4& mtx );
    void    SetP( const math::mat4& mtx );
    void    SetPV( const math::mat4& mtx );
    void    SetPVrotInv( const math::mat4& mtx );

    float       GetT() const;
    float       GetDT() const;
    math::mat4  GetV() const;
    math::mat4  GetVrot() const;
    math::mat4  GetP() const;
    math::mat4  GetPV() const;
    math::mat4  GetPVrotInv() const;

    static std::string GetUniformBlockLayout( GLuint shader_program );

protected:
    void SyncRAMBuffer();

private:
    void SetupBuffer();

    const uint64_t cOffsetT        = reinterpret_cast<uint64_t>(&mData.t)         - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetDt       = reinterpret_cast<uint64_t>(&mData.dt)        - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetV        = reinterpret_cast<uint64_t>(&mData.V)         - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetVrot     = reinterpret_cast<uint64_t>(&mData.Vrot)      - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetP        = reinterpret_cast<uint64_t>(&mData.P)         - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetPV       = reinterpret_cast<uint64_t>(&mData.PV)        - reinterpret_cast<uint64_t>(&mData);
    const uint64_t cOffsetPVrotInv = reinterpret_cast<uint64_t>(&mData.PVrotInv)  - reinterpret_cast<uint64_t>(&mData);

    // struct that mimics the exact memory layout of the std140 form of FrameInfo VRAM data
    // (contains unused, padding variables)
    struct FrameInfoStd140{
        float       t  = 0.0f;                             // offset: 0    ,size: 4
        float       dt = 0.0f;                             // offset: 4    ,size: 4
        const float paddingA[2] = {0.0f, 0.0f};
        math::mat4  V     = math::mat4::identity;          // offset: 16   ,size: 64
        math::mat4  Vrot  = math::mat4::identity;          // offset: 80   ,size: 64
        math::mat4  P     = math::mat4::identity;          // offset: 144  ,size: 64
        math::mat4  PV    = math::mat4::identity;          // offset: 208  ,size: 64
        math::mat4  PVrotInv = math::mat4::identity;       // offset: 272  ,size: 64

        FrameInfoStd140(){};
        FrameInfoStd140( const FrameInfoStd140& rhs );
        FrameInfoStd140& operator=( const FrameInfoStd140& rhs );
    };

    FrameInfoStd140 mData;

    bool mBufferIsDirty = true;
    gl::Buffer<FrameInfoStd140> mBuffer;
};

} // end of namespace 'gl'
} // end of namespace 'engine'
