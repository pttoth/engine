#include "engine/service/SystemManager.h"

#include "assimp/version.h"
#include "engine/gl/GlWrapper.h"
#include "engine/Services.h"
#include "engine/service/SystemControl.h"

#include "pt/alias.h"

#include "GL/glew.h"

#include "libpng/png.h"
#include "zlib.h"

#include <cassert>
#include <cstring>
#include <sstream>
#include <vector>

//--------------------------------------------------
//  Custom definitions
//--------------------------------------------------

#define PT_GLQueryEntry(MACRO_NAME) \
    GLQueryEntry( MACRO_NAME, #MACRO_NAME )

struct GLQueryEntry
{
    GLint macro;
    const char* txt;
    int result;
    GLQueryEntry( GLint m, const char* t ):
        macro(m), txt(t), result(-1)
    {}
};


// Appends padding to a string without making dynamic allocations
//   Uses a one-time use round-buffer.
//   Assumes the start of the buffer won't be in use by the time of recycling.
//   Many concurrent calls may fill up the buffer, resizing is a good short-term solution.
// If padding is added, the string is copied once, without dynamic allocation.
// If padding is not needed, it returns the input string.
// @TODO: rewrite later
// @HAZARD
const char*
StringWithPadding( const char* str, size_t preferred_length )
{
    // one-time use round buffer
    //   returned char* should not be stored (great for short-lifecycle rvalue usage)
    //   may present issues with heavy multithreaded use
    //      increasing buffer size helps on the short term
    //      otherwise rewrite - especially when you have your own memory manager with frame-long lifecycle :)
    static const size_t     stPaddingBufferSize = 4 * 4096;
    static       char       stPaddingBuffer[stPaddingBufferSize];
    static       std::mutex stPaddingMutex;
    static       size_t     stPaddingIndex = 0;

    if( 512 < preferred_length ){       // max padding size
        return str;
    }else if( stPaddingBufferSize < preferred_length ){ // if doesn't fit buffer, return the input string itself
        return str;
    }

    size_t len = strlen( str );
    if( preferred_length < len ){ // if larger than the padding needed, return the input string itself
        return str;
    }

    char* buf = 0;
    {
        pt::MutexLockGuard guard( stPaddingMutex );
        // this just rolls the buffer around, it may write over data in use, if the buffer got filled up quick
        //  with short memory lifecycle it doesn't need to
        if( stPaddingIndex + preferred_length +1 < stPaddingBufferSize ){ // if there's enough space left in buffer
            buf = stPaddingBuffer + stPaddingIndex;
        }else{      // if reached end of buffer, roll back around
            buf = stPaddingBuffer;
            stPaddingIndex = 0;
        }
        stPaddingIndex += preferred_length + 1; // advance the free memory index for next caller
    }

    std::memcpy( buf, str, len );                   // useful string data
    for( size_t i=len; i<preferred_length; ++i ){   // padding
        buf[i] = ' ';
    }
    buf[preferred_length] = 0;                      // terminating null

    return buf;
}


//--------------------------------------------------

using namespace engine;




SystemManager::
SystemManager()
{}


SystemManager::
~SystemManager()
{}


std::string SystemManager::
GetAssimpInfo() const
{
    std::ostringstream oss;
    oss << "v" << aiGetVersionMajor() << "." << aiGetVersionMinor() << "." << aiGetVersionPatch() << "." << aiGetVersionRevision();
    //oss << ", branch: '" << aiGetBranchName();
    return oss.str();
}


std::string SystemManager::
GetLibPNGInfo() const
{
    std::stringstream ss;
    ss << "Compiled with libpng " << PNG_LIBPNG_VER_STRING << "; using libpng " << png_libpng_ver << ".";
    return ss.str();
}


std::string SystemManager::
GetCPUInfo() const
{
    static const std::string retval( "Info query not implemented yet." );
    return retval;
}


std::string SystemManager::
GetGPUInfo() const
{
    static const std::string retval( "Info query not implemented yet." );
    return retval;
}


std::string SystemManager::
GetGraphicsAPIInfo() const
{
    int majorVersion = 0, minorVersion = 0;
    std::stringstream ss;
    ss << "GL Vendor            : " << glGetString( GL_VENDOR ) << "\n";
    ss << "GL Renderer          : " << glGetString( GL_RENDERER ) << "\n";
    ss << "GL Version (string)  : " << glGetString( GL_VERSION ) << "\n";
    glGetIntegerv( GL_MAJOR_VERSION, &majorVersion );
    glGetIntegerv( GL_MINOR_VERSION, &minorVersion );
    ss << "GL Version (integer) : " << majorVersion << "." << minorVersion << "\n";
    ss << "GLSL Version         : " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";
    ss << "Video Memory Total   : " << GetStrTotalCombinedVideoMemory() << "\n";
    ss << "VRAM Total           : " << GetStrVRAMTotal() << "\n";
    ss << "VRAM Available       : " << GetStrVRAMAvailable();
    return ss.str();
}


std::string SystemManager::
GetPlatformSpecificParameters() const
{
    std::vector<GLQueryEntry> entries;
    std::stringstream ss;
    entries.reserve(32);

    auto sc = Services::GetSystemControl();
    size_t padding = 44;

    entries.push_back( PT_GLQueryEntry( GL_MAX_UNIFORM_BUFFER_BINDINGS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMBINED_UNIFORM_BLOCKS ) );
    entries.push_back( GLQueryEntry( 0, "" ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_VERTEX_UNIFORM_BLOCKS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_GEOMETRY_UNIFORM_BLOCKS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_FRAGMENT_UNIFORM_BLOCKS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMPUTE_UNIFORM_BLOCKS ) );
    entries.push_back( GLQueryEntry( 0, "" ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_UNIFORM_BLOCK_SIZE ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS ) );

    entries.push_back( PT_GLQueryEntry( GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT ) );

    //entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_UNITS ) );   // deprecated: pre-GL2.0: fixed pipeline

    entries.push_back( GLQueryEntry( 0, "" ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_IMAGE_UNITS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_COORDS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_LOD_BIAS ) );
    entries.push_back( PT_GLQueryEntry( GL_NUM_COMPRESSED_TEXTURE_FORMATS  ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_SIZE ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_TEXTURE_BUFFER_SIZE ) );
    entries.push_back( GLQueryEntry( 0, "" ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_RENDERBUFFER_SIZE ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_COLOR_ATTACHMENTS ) );
    entries.push_back( PT_GLQueryEntry( GL_MAX_DRAW_BUFFERS ) );


    entries.push_back( GLQueryEntry( 0, "" ) );
    //entries.push_back( PT_GLQueryEntry(  ) );




    //entries.push_back( PT_GLQueryEntry( GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH ) );
    //entries.push_back( PT_GLQueryEntry( GL_ACTIVE_UNIFORM_BLOCKS ) );

    for( size_t i=0; i<entries.size(); ++i ){
        GLQueryEntry& e = entries[i];
        if( 0 == e.macro ){
            ss << "-----\n";
        }else{
            gl::GetIntegerv( e.macro, &(e.result) );
            ss << StringWithPadding( e.txt, padding ) << ": " << e.result;
            if( i < entries.size() -1 ){
                ss << "\n";
            }
        }
    }

    return ss.str();
}


std::string SystemManager::
GetZLibInfo() const
{
    std::stringstream ss;
    ss << "Compiled with zlib " << ZLIB_VERSION << "; using zlib " << zlib_version << ".";
    return ss.str();
}


const Version& SystemManager::
GetDXVersion() const
{
    return mDXVersion;
}


const Version& SystemManager::
GetGLVersion() const
{
    return mGLVersion;
}


const Version& SystemManager::
GetVKVersion() const
{
    return mVKVersion;
}


std::string SystemManager::
GetStrTotalCombinedVideoMemory() const
{
    return "n/a";
}


std::string SystemManager::
GetStrVRAMTotal() const
{
    int32_t totalVRAM = GetVRAMTotal();
    if( -1 == totalVRAM ){
        return "n/a";
    }
    std::stringstream ss;
    ss << totalVRAM /1024 << " MB";
    return ss.str();
}


std::string SystemManager::
GetStrVRAMAvailable() const
{
    int32_t availableVRAM = GetVRAMAvailable();
    if( -1 == availableVRAM ){
        return "n/a";
    }
    std::stringstream ss;
    ss << availableVRAM /1024 << " MB";
    return ss.str();
}


std::string SystemManager::
GetStrVRAMEngineUsage() const
{
    int32_t usedVRAM = GetVRAMEngineUsage();
    if( -1 == usedVRAM ){
        return "n/a";
    }
    std::stringstream ss;
    ss << usedVRAM /1024 << " MB";
    return ss.str();
}


size_t SystemManager::
GetMaximumUniformBlockBindingPoints() const
{
    return mMaxUniformBlockBindingPoints;
}


int SystemManager::
GetMaximumUniformBlocksCombined() const
{
    return mMaxUniformBlocksCombined;
}


int SystemManager::
GetMaximumUniformBlocksCompute() const
{
    return mMaxUniformBlocksCompute;
}


int SystemManager::
GetMaximumUniformBlocksFragment() const
{
    return mMaxUniformBlocksFragment;
}


int SystemManager::
GetMaximumUniformBlocksGeometry() const
{
    return mMaxUniformBlocksGeometry;
}


int SystemManager::
GetMaximumUniformBlocksTessControl() const
{
    return mMaxUniformBlocksTessControl;
}


int SystemManager::
GetMaximumUniformBlocksTessEval() const
{
    return mMaxUniformBlocksTessEval;
}


int SystemManager::
GetMaximumUniformBlocksVertex() const
{
    return mMaxUniformBlocksVertex;
}


int SystemManager::
GetVRAMTotal() const
{
#if defined GL_NVX_gpu_memory_info
    {
        // https://registry.khronos.org/OpenGL/extensions/NVX/NVX_gpu_memory_info.txt
        int32_t  videoMemoryTotal = -1;
        gl::GetIntegerv( GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &videoMemoryTotal );
        if( GL_NO_ERROR == gl::GetError() ){
            return videoMemoryTotal;
        }
    }
#endif
#if defined WGL_AMD_gpu_association
    {
        // TODO: get platform to test on! (Windows with AMD gpu)
        // https://registry.khronos.org/OpenGL/extensions/AMD/WGL_AMD_gpu_association.txt
        /*
        int32_t gpu_count = -1;
        int32_t gpu_max_count = 16;
        int32_t ids[gpu_max_count];
        gpu_count = wglGetGPUIDsAMD( gpu_max_count, ids );

        PT_LOG_DEBUG( "gpu_count: " << gpu_count );
        for( int32_t i=0; i<gpu_max_count;++i ){
            PT_LOG_DEBUG( "ids["<<i<<"] : " << ids[i] );
        }

        int buf[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
        int retval = -1;

        //retval = wglGetGPUInfoAMD( ids[0], WGL_GPU_RAM_AMD, GL_UNSIGNED_INT, 10, buf);
        retval = wglGetGPUInfoAMD( ids[0], WGL_GPU_RAM_AMD, GL_INT, 10, buf);

        if( GL_NO_ERROR == gl::GetError() ){
            return retval * 1024; // 'WGL_GPU_RAM_AMD' returns MBytes, the function returns kbytes
        }
        */
    }
#endif
    return -1;
}


int SystemManager::
GetVRAMAvailable() const
{
#if defined GL_NVX_gpu_memory_info
    {
        // https://registry.khronos.org/OpenGL/extensions/NVX/NVX_gpu_memory_info.txt
        int videoMemoryAvailable = -1;
        gl::GetIntegerv( GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &videoMemoryAvailable );
        if( GL_NO_ERROR == gl::GetError() ){
            return videoMemoryAvailable;
        }
    }
#endif
#if defined GL_ATI_meminfo
    {
        // https://registry.khronos.org/OpenGL/extensions/ATI/ATI_meminfo.txt
        // [0]: total memory free in the pool
        // [1]: largest available free block in the pool
        // [2]: total auxiliary memory free
        // [3]: largest auxiliary free block
        int videoMemoryAvailable[4] = {-1,-1,-1,-1};
        //gl::GetIntegerv( GL_VBO_FREE_MEMORY_ATI, videoMemoryAvailable );
        //gl::GetIntegerv( GL_TEXTURE_FREE_MEMORY_ATI, videoMemoryAvailable );
        gl::GetIntegerv( GL_RENDERBUFFER_FREE_MEMORY_ATI, videoMemoryAvailable );
        if( GL_NO_ERROR == gl::GetError() ){
            return videoMemoryAvailable[0];
        }
    }
#endif
    return -1;
}


int SystemManager::
GetVRAMEngineUsage() const
{
    return -1;
}


void SystemManager::
Initialize()
{
    if( !mInitialized ){
        int res = 0;
        gl::GetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, &res );
        assert( 0 <= res );
        mMaxUniformBlockBindingPoints = std::max( 0, res );

        gl::GetIntegerv( GL_MAX_COMBINED_UNIFORM_BLOCKS,        &mMaxUniformBlocksCombined );
        gl::GetIntegerv( GL_MAX_COMPUTE_UNIFORM_BLOCKS,         &mMaxUniformBlocksCompute );
        gl::GetIntegerv( GL_MAX_FRAGMENT_UNIFORM_BLOCKS,        &mMaxUniformBlocksFragment );
        gl::GetIntegerv( GL_MAX_GEOMETRY_UNIFORM_BLOCKS,        &mMaxUniformBlocksGeometry );
        gl::GetIntegerv( GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS,    &mMaxUniformBlocksTessControl );
        gl::GetIntegerv( GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS, &mMaxUniformBlocksTessEval );
        gl::GetIntegerv( GL_MAX_VERTEX_UNIFORM_BLOCKS,          &mMaxUniformBlocksVertex );
    }
}
