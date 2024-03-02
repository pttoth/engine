#include "engine/gl/Shader.h"

#include "engine/gl/GlWrapper.h"
#include "pt/guard.hpp"
#include "pt/logging.h"

//using namespace engine;
using namespace engine::gl;


Shader::
Shader::
Shader( const std::string& name, gl::ShaderType type, const ConstStdSharedPtr code ):
    mName( name ), mType( type ), mSourceCode( code )
{}


Shader::
~Shader()
{
    FreeVRAM();
}


Shader& Shader::
operator=( Shader&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


bool Shader::
Compile()
{
    if( 0 == mHandle ){
        PT_LOG_OUT( "Compiling shader '" << mName << "' (type: " << mType << ")" );
        GLenum  errorcode = 0;
        GLint   success   = GL_FALSE;

        // leaving the case of mType='ShaderType::NO_SHADER_TYPE' to be error-checked by OpenGL
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

        const GLchar* sourcecode = mSourceCode->c_str();
        const GLint   length     = mSourceCode->length();

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
        if( !success ){
            PT_LOG_ERR( "Failed to compile shader '" << mName << "'" );
            gl::PrintShaderProgramInfoLog( mHandle );
            return false;
        }

        guardHandle.Disable();
    }

    return true;
}


void Shader::
FreeVRAM()
{
    if( 0 != mHandle ){
        PT_LOG_DEBUG( "Deleting shader '" << mName << "'" );
        gl::DeleteShader( mHandle );
#ifdef ENGINE_DEBUG_ENABLED
        GLenum  errorcode = gl::GetError();
        if( GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "Could not delete shader '" << mName
                        << "' (type: " << std::hex << mType << std::dec
                        << ")\n  (" << gl::GetErrorString( errorcode )
                        << "):\n" << gl::GetErrorDescription( errorcode ) )
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


pt::Name Shader::
GetName() const
{
    return mName;
}


ShaderType Shader::
GetShaderType() const
{
    return mType;
}


ConstStdSharedPtr Shader::
GetSourceCode() const
{
    return mSourceCode;
}


bool Shader::
IsCompiled() const
{
    return ( 0 != mHandle );
}


