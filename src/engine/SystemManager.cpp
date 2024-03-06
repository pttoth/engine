#include "engine/SystemManager.h"

#include "GL/glew.h"
#include "png.h"
#include "zlib.h"

#include <sstream>

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
    static const std::string retval( "Info query not implemented yet." );
    return retval;
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
    ss << "GLSL Version : " << glGetString( GL_SHADING_LANGUAGE_VERSION );
    return ss.str();
}


std::string SystemManager::
GetZLibInfo() const
{
    std::stringstream ss;
    ss << "Compiled with zlib " << ZLIB_VERSION << "; using zlib " << zlib_version << ".";
    return ss.str();
}
