#include "engine/gl/Shader.h"

#include "engine/gl/GlWrapper.h"
#include "pt/guard.hpp"
#include "pt/logging.h"

//using namespace engine;
using namespace engine::gl;


Shader::
Shader::
Shader( const pt::Name& name, gl::ShaderType type, const ConstStdStringPtr code ):
    mName( name ), mType( type ), mSourceCode( code )
{}


Shader::
~Shader()
{
    FreeVRAM();
}


Shader::
Shader( Shader&& source ):
    mName( std::move(source.mName) ), mType( source.mType ),
    mHandle( source.mHandle ), mSourceCode( source.mSourceCode )
{
    source.mType = gl::ShaderType::NO_SHADER_TYPE;
    source.mHandle = 0;
    source.mSourceCode = nullptr;
}


Shader& Shader::
operator=( Shader&& source )
{
    if( this != &source ){
        mName       = source.mName;
        mType       = source.mType;
        mHandle     = source.mHandle;
        mSourceCode = source.mSourceCode;

        source.mName = pt::Name();
        source.mType = gl::ShaderType::NO_SHADER_TYPE;
        source.mHandle = 0;
        source.mSourceCode = nullptr;
    }
    return *this;
}


bool Shader::
Compile()
{
    if( 0 == mHandle ){
        PT_LOG_DEBUG( "Creaing shader object for '" << mName.GetStdString() << "' (type: " << gl::GetShaderTypeAsString(mType) << ")" );
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
            PT_LOG_ERR( "Could not create shader '" << mName.GetStdString()
                        << "' (type: " << std::hex << mType << std::dec
                        << ")\n  (" << gl::GetErrorString( errorcode )
                        << "):\n  " << gl::GetErrorDescription( errorcode ) );
            return false;
        }

        PT_LOG_OUT( "Compiling shader(" << mHandle << ")'" << mName.GetStdString() << "' (type: " << gl::GetShaderTypeAsString(mType) << ")" );

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
        if( GL_FALSE == success ){
            PT_LOG_ERR( "Failed to compile shader '" << mName.GetStdString() << "'" );
            gl::PrintShaderInfoLog( mHandle );
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
        PT_LOG_DEBUG( "Deleting shader(" << mHandle << ")'" << mName.GetStdString() << "'" );
        gl::DeleteShader( mHandle );
#ifdef ENGINE_DEBUG_ENABLED
        GLenum  errorcode = gl::GetError();
        if( GL_NO_ERROR != errorcode ){
            PT_LOG_ERR( "Could not delete shader '" << mName.GetStdString()
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


