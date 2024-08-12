/** -----------------------------------------------------------------------------
  * FILE:    Buffer.hpp
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Manages GPU buffer lifecycle and data transmission to them.
  *          Defines additional, templated versions of GL calls for convenient use.
  * -----------------------------------------------------------------------------
  */

// TODO: add support for BufferSubData
// TODO: add support for uploading outside data without holding local copy
//          see: Buffer( size_t length, const uint8_t* data );

#pragma once

#include "GlWrapper.h"

#include "engine/Utility.h"
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


    Buffer( size_t length, const T* data )
    {
        if( nullptr == data ){
            PT_LOG_ERR( "Tried to create Buffer with 'nullptr' as data!" );
            PT_PRINT_DEBUG_STACKTRACE();
            return;
        }

        if( 0 == length ){
            PT_LOG_ERR( "Tried to create Buffer from data with explicit zero size!" );
            PT_PRINT_DEBUG_STACKTRACE();
            return;
        }

        mData.resize( length );
        for( size_t i=0; i<length; ++i ){
            mData[i] = data[i];
        }
    }


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
            PT_LOG_DEBUG_GL_BUFFER( "Freeing up clientside data for buffer( " << mBufferID << " | elements: " << mData.size() << " | " << GetBytes() << " bytes )" );
        }
        mData = std::vector<T>();
        mDirty = true;
    }


    void FreeVRAM()
    {
        if( 0 < mBufferID ){
            PT_LOG_DEBUG_GL_BUFFER( "Freeing up VRAM buffer( " << mBufferID << " | " << mVRAMbytes << " bytes )..." );
            gl::DeleteBuffers( 1, &mBufferID );
            mBufferID = 0;
            mVRAMbytes = 0;
        }
        mDirty = true;
    }


    inline const std::vector<T>& GetDataRef() const
    {
        return mData;
    }


    inline std::vector<T>& GetDataRef()
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

        // avoid per-frame log flooding
        #ifdef PT_DEBUG_ENABLED
            if( PT_BUFFER_DYNAMIC_CONDITION_FILTER( hint ) && PT_BUFFER_STREAM_CONDITION_FILTER( hint ) ){
                PT_LOG_DEBUG_GL_BUFFER( "Buffering data( " << mBufferID << " | '" << gl::GetBufferTargetAsString(target) << "' | elements: " << mData.size() << " | " << GetBytes() << " bytes ) to VRAM..." );
            }
        #endif

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

template< class T >
void BindBufferBase( GLenum target, GLuint index, const gl::Buffer<T>& buffer )
{
    gl::BindBufferBase( target, index, buffer.GetHandle() );
}

} // end of namespace 'gl'
} // end of namespace 'engine'
