#include "engine/gl/Shader.h"

#include "engine/gl/GlWrapper.h"
#include "engine/Utility.h"
#include "pt/guard.hpp"
#include "pt/logging.h"

using namespace engine::gl;


Shader::
~Shader()
{
    FreeVRAM();
}


ShaderPtr Shader::
CreateFromFile( const std::string& name, gl::ShaderType type, const std::string& path )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Tried to create shader with empty name!" );
        return nullptr;
    }

    if( 0 == path.length() ){
        PT_LOG_ERR( "Tried to create shader '" << name << "' from empty path!" );
        return nullptr;
    }

    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Could not load shader file '" << path << "'!" );
        return nullptr;
    }

    std::stringstream ss;
    {
        const size_t bufsize = 16 * 1024 + 1;
        char buf[bufsize];
        buf[bufsize-1] = 0;
        while( ifs.good() ){
            ifs.read( buf, bufsize-1 );
            size_t chars_read = ifs.gcount();
            buf[chars_read] = 0;
            ss << buf;
        }
    }

    if( 0 == ss.str().length() ){
        PT_LOG_ERR( "Failed to load shader code for '" << name << "'(path: '" << path << "')!" );
        return nullptr;
    }

    ShaderPtr instance  = ShaderPtr( new Shader() );
    instance->mName     = name;
    instance->mType     = type;
    instance->mPath     = path;
    instance->mSourceCode = NewPtr<const std::string>( ss.str() );

    PT_LOG_INFO( "Loaded '" << GetShaderTypeAsString(type) << "' code for '" << name << "' from file '" << path << "'" );

    return instance;
}


bool Shader::
Compile()
{
    // if failed compilation once, don't even try again
    if( mFailedCompilation ){
        PT_LOG_ERR( "Tried to compile shader '" << mName << "', that already failed compilation once!" );
        return false;
    }

    if( 0 == mHandle ){
        PT_LOG_DEBUG( "Creaing shader object for '" << mName << "' (type: " << gl::GetShaderTypeAsString(mType) << ")" );
        GLenum  errorcode = 0;
        GLint   success   = GL_FALSE;

        // leaving the case of mType='ShaderType::NO_SHADER_TYPE' to be error-checked as OpenGL error
        mHandle = gl::CreateShader( mType );
        auto guardHandle = pt::CreateGuard( [this]{
            gl::DeleteShader( mHandle );
            mHandle = 0;
        } );

        errorcode = gl::GetError();
        if( 0 == mHandle || GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "Could not create shader '" << mName
                        << "' (type: " << std::hex << mType << std::dec
                        << ")\n  (" << gl::GetErrorString( errorcode )
                        << "):\n  " << gl::GetErrorDescription( errorcode ) );
            return false;
        }

        PT_LOG_OUT( "Compiling shader(" << mHandle << ")'" << mName << "' (type: " << gl::GetShaderTypeAsString(mType) << ")" );

        const GLchar* sourcecode = mSourceCode->c_str();
        const GLint   length     = mSourceCode->length();

/*
#ifdef PT_DEBUG_ENABLED
        PT_LOG_INFO( "-----\nShader code:\n-----\n" << sourcecode );
#endif
*/

        // upload source code to VRAM
        gl::ShaderSource( mHandle, 1, &sourcecode, &length );
        errorcode = gl::GetError();
        if( GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "OpenGL error: " << gl::GetErrorString( errorcode )
                        << "\n Description:  " << gl::GetErrorDescription( errorcode ) );
            return false;
        }

        gl::CompileShader( mHandle );
        gl::GetShaderiv( mHandle, GL_COMPILE_STATUS, &success);
        if( GL_FALSE == success ){
            mFailedCompilation = true;
            PT_LOG_ERR( "Failed to compile shader '" << mName << "'" );
            gl::PrintShaderInfoLog( mHandle );
            return false;
        }

/*
#ifdef PT_DEBUG_ENABLED
        PT_LOG_INFO( "-----" );
#endif
*/

        guardHandle.Disable();
    }

    return true;
}


void Shader::
FreeVRAM()
{
    if( 0 != mHandle ){
        PT_LOG_DEBUG( "Deleting shader(" << mHandle << ")'" << mName << "'" );
        gl::DeleteShader( mHandle );
#ifdef ENGINE_DEBUG_ENABLED
        GLenum  errorcode = gl::GetError();
        if( GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "Could not delete shader '" << mName
                        << "' (type: " << std::hex << mType << std::dec
                        << ")\n  (" << gl::GetErrorString( errorcode )
                        << "):\n" << gl::GetErrorDescription( errorcode ) );
        }

        GLint result = GL_FALSE;
        gl::GetShaderiv( mHandle, GL_DELETE_STATUS, &result );
        if( GL_TRUE == result ){
            PT_LOG_WARN( "Deleted shader '" << mHandle << "' is still bound to a shaderprogram and was only flagged for deletion!" );
        }
#endif
        mHandle = 0;
    }
}


GLuint Shader::
GetHandle() const
{
    return mHandle;
}


std::string Shader::
GetName() const
{
    return mName;
}


ShaderType Shader::
GetShaderType() const
{
    return mType;
}


ConstStdStringPtr Shader::
GetSourceCode() const
{
    return mSourceCode;
}


bool Shader::
IsCompiled() const
{
    return ( 0 != mHandle );
}


Shader::
Shader()
{}


Shader::
Shader( const std::string& name, gl::ShaderType type, const ConstStdStringPtr code ):
    mName( name ), mType( type ), mSourceCode( code )
{}
