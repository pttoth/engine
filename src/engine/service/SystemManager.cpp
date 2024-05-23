#include "engine/service/SystemManager.h"

#include "assimp/version.h"
#include "GL/glew.h"
#include "png.h"
#include "zlib.h"

#include <sstream>

#if defined PT_PLATFORM_WINDOWS
#include "GL/wglew.h"
#endif

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
    ss << "GL Vendor    : " << glGetString( GL_VENDOR ) << "\n";
    ss << "GL Renderer  : " << glGetString( GL_RENDERER ) << "\n";
    ss << "GL Version (string)  : " << glGetString( GL_VERSION ) << "\n";
    glGetIntegerv( GL_MAJOR_VERSION, &majorVersion );
    glGetIntegerv( GL_MINOR_VERSION, &minorVersion );
    ss << "GL Version (integer) : " << majorVersion << "." << minorVersion << "\n";
    ss << "GLSL Version         : " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << "\n";
    ss << "VRAM Total     : " << GetStrVRAMTotal() << "\n";
    ss << "VRAM Available : " << GetStrVRAMAvailable();
    return ss.str();
}


std::string SystemManager::
GetZLibInfo() const
{
    std::stringstream ss;
    ss << "Compiled with zlib " << ZLIB_VERSION << "; using zlib " << zlib_version << ".";
    return ss.str();
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


int SystemManager::
GetVRAMTotal() const
{
#if defined GL_NVX_gpu_memory_info
    {
        // https://registry.khronos.org/OpenGL/extensions/NVX/NVX_gpu_memory_info.txt
        int32_t  videoMemoryTotal = -1;
        glGetIntegerv( GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &videoMemoryTotal );
        if( GL_NO_ERROR == glGetError() ){
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

        if( GL_NO_ERROR == glGetError() ){
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
        glGetIntegerv( GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &videoMemoryAvailable );
        if( GL_NO_ERROR == glGetError() ){
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
        //glGetIntegerv( GL_VBO_FREE_MEMORY_ATI, videoMemoryAvailable );
        //glGetIntegerv( GL_TEXTURE_FREE_MEMORY_ATI, videoMemoryAvailable );
        glGetIntegerv( GL_RENDERBUFFER_FREE_MEMORY_ATI, videoMemoryAvailable );
        if( GL_NO_ERROR == glGetError() ){
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
