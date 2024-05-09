/** -----------------------------------------------------------------------------
  * FILE:    Buffer.hpp
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Manages GPU buffer lifecycle and data transmission to them.
  *          Defines additional, templated versions of GL calls for convenient use.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "GlWrapper.h"

#include "pt/logging.h"
#include "pt/macros.h"
#include <assert.h>
#include <memory>
#include <vector>

// control logic of verbose debug logging
#ifdef ENGINE_GL_DEBUG_BUFFER
    #define PT_LOG_DEBUG_GL_BUFFER(expr) pt::log::debug << expr << std::endl
#else
    #define PT_LOG_DEBUG_GL_BUFFER(expr) (__PT_VOID_CAST (0))
#endif

namespace engine{
namespace gl{

template<class T> class Buffer;
template<class T> using BufferPtr       = std::shared_ptr< Buffer<T> >;
template<class T> using ConstBufferPtr  = std::shared_ptr< const Buffer<T> >;
template<class T> using BufferWPtr      = std::weak_ptr< Buffer<T> >;
template<class T> using ConstBufferWPtr = std::weak_ptr< const Buffer<T> >;
template<class T> using BufferUPtr      = std::unique_ptr< Buffer<T> >;
template<class T> using ConstBufferUPtr = std::unique_ptr< const Buffer<T> >;

template< class T >
class Buffer
{
public:
    Buffer(){}
    virtual ~Buffer(){
        FreeVRAM();
    }
    Buffer( const Buffer& other ) = delete;             // prevent copying VRAM handle
    Buffer& operator=( const Buffer& other ) = delete;  // prevent copying VRAM handle
    Buffer( Buffer&& source ):
        mBufferID( source.mBufferID ),
        mVRAMbytes( source.mVRAMbytes ),
        mData( std::move(source.mData) ),
        mDirty( source.mDirty )
    {
        source.SetDefaultMemberValues();
    }

    Buffer& operator=( Buffer&& source ){
        if( this != &source ){
            FreeVRAM();
            FreeClientsideData();

            mBufferID = source.mBufferID;
            mVRAMbytes = source.mVRAMbytes;
            mData = std::move( source.mData );
            mDirty = source.mDirty;

            source.SetDefaultMemberValues();
        }
        return *this;
    }

    bool operator==( const Buffer& other ) const = delete;

    Buffer( const std::initializer_list<T>& data ):
        mData( data )
    {}


    Buffer( const std::vector<T>& data ):
        mData( data )
    {}


    Buffer( std::vector<T>&& data ):
        mData( std::move( data ) )
    {}


    Buffer& operator=( const std::vector<T>& data )
    {
        mData = data;
        mDirty = true;
        return *this;
    }


    Buffer& operator=( std::vector<T>&& data )
    {
        mData = std::move( data );
        mDirty = true;
        return *this;
    }


    bool IsClientSideSynced()
    {
        return !mDirty;
    }


    void FreeClientsideData()
    {
        if( 0 != mData.capacity() ){
            PT_LOG_DEBUG_GL_BUFFER( "Freeing up clientside data for buffer(" << mBufferID << ", elements: " << mData.size() << ", bytes: " << GetBytes() << ")" );
        }
        mData = std::vector<T>();
        mDirty = true;
    }


    void FreeVRAM()
    {
        if( 0 < mBufferID ){
            PT_LOG_DEBUG_GL_BUFFER( "Freeing up VRAM buffer(" << mBufferID << ", size: " << mVRAMbytes << ")..." );
            gl::DeleteBuffers( 1, &mBufferID );
            mBufferID = 0;
            mVRAMbytes = 0;
        }
        mDirty = true;
    }


    inline const std::vector<T>& GetData() const
    {
        return mData;
    }


    inline std::vector<T> GetData()
    {
        return mData;
    }


    inline GLuint GetHandle() const
    {
        return mBufferID;
    }


    inline size_t GetBytes() const
    {
        return sizeof(T) * mData.size();
    }


    inline size_t GetVRAMBytes() const
    {
        return mVRAMbytes;
    }


    void LoadToVRAM( BufferTarget target, BufferHint hint )
    {
        if( 0 == mBufferID ){
            gl::GenBuffers( 1, &mBufferID );
            PT_LOG_DEBUG_GL_BUFFER( "Generated new buffer(" << mBufferID << ") on GPU" );
            if( 0 == mBufferID ){
                const char* errormsg = "Failed to generate GL buffer!";
                PT_LOG_ERR( errormsg );
                assert( 0 < mBufferID );
            }
        }

        PT_LOG_DEBUG_GL_BUFFER( "Buffering data(" << mBufferID << ", '" << gl::GetBufferTargetAsString(target) << "', elements: " << mData.size() << ", bytes: " << GetBytes() << ") to VRAM..." );
        gl::BindBuffer( target, mBufferID );
        gl::BufferData( target, mData.size()*sizeof(T), mData.data(), hint );
        PT_GL_UnbindBuffer( target );
        mVRAMbytes = this->GetBytes();
        mDirty = false;
    }

private:
    void SetDefaultMemberValues(){
        mBufferID   = 0;
        mVRAMbytes  = 0;
        mData       = std::vector<T>();
        mDirty      = true;
    }
    GLuint          mBufferID = 0;
    size_t          mVRAMbytes = 0;
    std::vector<T>  mData;
    bool            mDirty = true;
};



template< class T >
void BindBuffer( GLenum target, const gl::Buffer<T>& buffer )
{
    gl::BindBuffer( target, buffer.GetHandle() );
}


} // end of namespace 'gl'
} // end of namespace 'engine'
