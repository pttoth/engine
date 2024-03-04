#include "engine/SystemManager.h"

#include "png.h"
#include "zlib.h"

#include <sstream>

using namespace engine;

std::string SystemManager::
GetAssimpInfo() const
{
    static const std::string retval( "Info query not implemented yet." );
    return retval;
}


std::string SystemManager::
GetLibPNGInfo() const
{
    std::stringstream ss;
    ss << "Compiled with libpng " << PNG_LIBPNG_VER_STRING << "; using libpng " << png_libpng_ver << ".\n";
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
    static const std::string retval( "Info query not implemented yet." );
    return retval;
}


std::string SystemManager::
GetZLibInfo() const
{
    std::stringstream ss;
    ss << "Compiled with zlib " << ZLIB_VERSION << "; using zlib " << zlib_version << ".\n";
    return ss.str();
}
