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
    #define PT_LOG_DEBUG_GL_BUFFER(expr) pt::log::debug << expr << std::endl;
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
    Buffer( Buffer&& source ) = default;
    Buffer& operator=( Buffer&& source ) = default;
    bool operator==( const Buffer& other ) const = delete; // no copy -> no equality

    Buffer( const std::initializer_list<T> data ):
        mData( data )
    {
        PT_LOG_DEBUG_GL_BUFFER( "Copying initializer list data to fill buffer (bytes: " << data.size() << ")" );
    }


    Buffer( const std::vector<T>& data ):
        mData( data )
    {}


    Buffer( std::vector<T>&& data ):
        mData( std::move( data ) )
    {}


    Buffer& operator=( const std::vector<T>& data )
    {
        PT_LOG_DEBUG_GL_BUFFER( "Copying vector data to fill buffer (bytes: " << data.size() << ")" );
        mData = data;
    }


    Buffer& operator=( std::vector<T>&& data )
    {
        PT_LOG_DEBUG_GL_BUFFER( "Moving vector data to fill buffer (bytes: " << data.size() << ")" );
        mData = std::move( data );
    }


    void FreeClientsideData()
    {
        PT_LOG_DEBUG_GL_BUFFER( "Freeing up clientside data for buffer '" << mBufferID << "' (size: " << this->GetSize() << ")" );
        mData.clear();
        mData.shrink_to_fit();
    }


    void FreeVRAM()
    {
        if( 0 < mBufferID ){
            PT_LOG_DEBUG_GL_BUFFER( "Freeing up VRAM buffer '" << mBufferID << "' (size: " << mVRAMbytes << ")..." );
            gl::DeleteBuffers( 1, &mBufferID );
            mBufferID = 0;
            mVRAMbytes = 0;
        }
    }


    inline GLuint GetHandle() const
    {
        return mBufferID;
    }


    inline size_t GetSize() const
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
            PT_LOG_DEBUG_GL_BUFFER( "Generating new buffer on GPU..." );
            gl::GenBuffers( 1, &mBufferID );
            PT_LOG_DEBUG_GL_BUFFER( "  New buffer handle: " << mBufferID );
            assert( 0 < mBufferID );
            if( 0 == mBufferID ){
                const char* errormsg = "Failed to generate GL buffer!";
                pt::log::err << errormsg;
                throw std::runtime_error( errormsg );
            }
        }

        PT_LOG_DEBUG_GL_BUFFER( "Loading buffer '" << mBufferID << "'(size: " << this->GetSize() << ") to VRAM..." );
        gl::BindBuffer( target, mBufferID );
        gl::BufferData( target, mData.size(), mData.data(), hint );
        gl::BindBuffer( target, 0 );
        mVRAMbytes = this->GetSize();
    }

private:
    GLuint           mBufferID = 0;
    size_t           mVRAMbytes = 0;
    std::vector<T>   mData;
};



template< class T >
void BindBuffer( GLenum target, const gl::Buffer<T>& buffer )
{
    GLuint id = buffer.GetHandle();
    assert( 0 < id );
    gl::BindBuffer( target, id );
}


} // end of namespace 'gl'
} // end of namespace 'engine'
