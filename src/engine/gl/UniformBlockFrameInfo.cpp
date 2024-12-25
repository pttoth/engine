#include "engine/gl/UniformBlockFrameInfo.h"

#include "engine/StandardShaderProgram.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"
#include "engine/service/SystemControl.h"

using namespace engine;
using namespace engine::gl;

//--------------------------------------------------
//  static functions
//--------------------------------------------------


//--------------------------------------------------
//  member functions
//--------------------------------------------------


UniformBlockFrameInfo::
UniformBlockFrameInfo()
{
    SetupBuffer();
}


UniformBlockFrameInfo::
~UniformBlockFrameInfo()
{}


UniformBlockFrameInfo::
UniformBlockFrameInfo( UniformBlockFrameInfo&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


UniformBlockFrameInfo& UniformBlockFrameInfo::
operator=( UniformBlockFrameInfo&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
    return *this;
}


void UniformBlockFrameInfo::
BindBufferToBindingPoint( GLuint index )
{
    auto sc = Services::GetSystemControl();

    size_t numBindingPoints = sc->GetMaximumUniformBlockBindingPoints();
    if( numBindingPoints <= index ){
        PT_LOG_ERR( "Tried to bind uniform block to invalid binding point index (idx: " << index << ", max: " << numBindingPoints << ")." );
        return;
    }

    gl::BindBufferBase( gl::BufferTarget::UNIFORM_BUFFER, index, mBuffer );
}


bool UniformBlockFrameInfo::
IsClientSideSynced()
{
    return (!mBufferIsDirty) && (mBuffer.IsClientSideSynced());
}


void UniformBlockFrameInfo::
FreeVRAM()
{
    mBuffer.FreeVRAM();
}


GLuint UniformBlockFrameInfo::
GetHandle() const
{
    return mBuffer.GetHandle();
}


size_t UniformBlockFrameInfo::
GetBytes() const
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
    return 0;
    //return sizeof(T) * mData.size();
}


size_t UniformBlockFrameInfo::
GetVRAMBytes() const
{
    return mBuffer.GetVRAMBytes();
}


void UniformBlockFrameInfo::
LoadToVRAM( BufferTarget target, BufferHint hint )
{
    SyncRAMBuffer();
    mBuffer.LoadToVRAM( target, hint );
    gl::UnbindBuffer( target );
}


void UniformBlockFrameInfo::
SetT( float val )
{
    mData.t = val;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetDT( float val )
{
    mData.dt = val;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetV( const math::mat4& mtx )
{
    mData.V = mtx;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetVrot( const math::mat4& mtx )
{
    mData.Vrot = mtx;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetP( const math::mat4& mtx )
{
    mData.P = mtx;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetPV( const math::mat4& mtx )
{
    mData.PV = mtx;
    mBufferIsDirty = true;
}


void UniformBlockFrameInfo::
SetPVrotInv( const math::mat4& mtx )
{
    mData.PVrotInv = mtx;
    mBufferIsDirty = true;
}


float UniformBlockFrameInfo::
GetT() const
{
    return mData.t;
}


float UniformBlockFrameInfo::
GetDT() const
{
    return mData.t;
}


math::mat4 UniformBlockFrameInfo::
GetV() const
{
    return mData.V;
}


math::mat4 UniformBlockFrameInfo::
GetVrot() const
{
    return mData.Vrot;
}


math::mat4 UniformBlockFrameInfo::
GetP() const
{
    return mData.P;
}


math::mat4 UniformBlockFrameInfo::
GetPV() const
{
    return mData.PV;
}


math::mat4 UniformBlockFrameInfo::
GetPVrotInv() const
{
    return mData.PVrotInv;
}


std::string UniformBlockFrameInfo::
GetUniformBlockLayout( GLuint shader_program )
{
    const size_t siz = 7;
    const GLchar* uniformNames[ siz ] = {
        "FrameInfo.t",
        "FrameInfo.dt",
        "FrameInfo.V",
        "FrameInfo.Vrot",
        "FrameInfo.P",
        "FrameInfo.PV",
        "FrameInfo.PVrotInv"
    };

    // These hold results from GL queries
    GLuint  uniformIndices[siz];
    GLint   uniformOffsets[siz];
    GLint   uniformArrayStrides[siz];
    GLint   uniformMatrixStrides[siz];

    // Built from 'uniformIndices' with unknown variables being filtered
    GLuint  uniformQueryIndices[siz];
    int32_t uniformQueryNameIndices[siz];
    int32_t uniformQueryInvalidNameIndices[siz];
    size_t  uniformQuerySize = 0;
    size_t  uniformInvalidQuerySize = 0;

    std::stringstream ss;

    for( size_t i=0; i<siz; ++i ){
        uniformIndices[i] = -1; // should be 0 ?
        uniformOffsets[i] = -1;
        uniformArrayStrides[i] = -1;
        uniformMatrixStrides[i] = -1;
    }

    gl::GetUniformIndices( shader_program, siz, uniformNames, uniformIndices);

    // construct query
    for( size_t i=0; i<siz; ++i ){
        if( ((GLuint)-1) != uniformIndices[i] ){
            uniformQueryIndices[uniformQuerySize] = uniformIndices[i];
            uniformQueryNameIndices[uniformQuerySize] = i;
            ++uniformQuerySize;
        }else{
            uniformQueryInvalidNameIndices[uniformInvalidQuerySize] = i;
            ++uniformInvalidQuerySize;
        }
    }

    gl::GetActiveUniformsiv( shader_program, uniformQuerySize, uniformQueryIndices,
                           GL_UNIFORM_OFFSET, uniformOffsets );
    gl::GetActiveUniformsiv( shader_program, uniformQuerySize, uniformQueryIndices,
                           GL_UNIFORM_ARRAY_STRIDE, uniformArrayStrides );
    gl::GetActiveUniformsiv( shader_program, uniformQuerySize, uniformQueryIndices,
                           GL_UNIFORM_MATRIX_STRIDE, uniformMatrixStrides );

    for( size_t i=0; i<uniformQuerySize; ++i ){
        ss  << "idx:  " << uniformQueryIndices[i]
            << ",\toffset: " << uniformOffsets[i]
            << ",\t\ta stride: " << uniformArrayStrides[i]
            << ",\tm stride: " << uniformMatrixStrides[i]
            << ",\tname: " << uniformNames[ uniformQueryNameIndices[i] ]
            << "\n";
    }

    if( 0 < uniformInvalidQuerySize ){
        ss << "The following variable names were not found:\n  ";
        for( size_t i=0; i<uniformInvalidQuerySize; ++i ){
            if( 0 < i ){
              ss << ", ";
            }
            ss << "'" << uniformNames[ uniformQueryInvalidNameIndices[i] ] << "'";
        }
    }
    return ss.str();
}


void UniformBlockFrameInfo::
SyncRAMBuffer()
{
    // copy the memory image of the 'mData' struct into the 'mBuffer' member
    if( mBufferIsDirty ){
        FrameInfoStd140* pData = mBuffer.GetDataRef().data();
        pData[0] = mData;
        mBufferIsDirty = false;
    }
}


void UniformBlockFrameInfo::
SetupBuffer()
{
    PT_LOG_DEBUG( "cOffsetT: "      << cOffsetT );
    PT_LOG_DEBUG( "cOffsetDt: "     << cOffsetDt );
    PT_LOG_DEBUG( "cOffsetV: "      << cOffsetV );
    PT_LOG_DEBUG( "cOffsetVrot: "   << cOffsetVrot );
    PT_LOG_DEBUG( "cOffsetP: "      << cOffsetP );
    PT_LOG_DEBUG( "cOffsetPV: "     << cOffsetPV );
    PT_LOG_DEBUG( "cOffsetPVrotInv: " << cOffsetPVrotInv );

    mBuffer = gl::Buffer<FrameInfoStd140>( 1, &mData );
    mBufferIsDirty = false;
}


UniformBlockFrameInfo::FrameInfoStd140::
FrameInfoStd140( const FrameInfoStd140& rhs ):
    t( rhs.t), dt( rhs.dt ),
    V( rhs.V ), Vrot( rhs.Vrot ),
    P( rhs.P ), PV( rhs.PV ), PVrotInv( rhs.PVrotInv )
{}


UniformBlockFrameInfo::FrameInfoStd140& UniformBlockFrameInfo::FrameInfoStd140::
operator=( const FrameInfoStd140& rhs )
{
    t        = rhs.t;
    dt       = rhs.dt;
    V        = rhs.V;
    Vrot     = rhs.Vrot;
    P        = rhs.P;
    PV       = rhs.PV;
    PVrotInv = rhs.PVrotInv;
    return *this;
};
