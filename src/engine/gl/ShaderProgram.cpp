#include "engine/gl/ShaderProgram.h"

#include "engine/gl/GlWrapper.h"
#include "engine/DrawingControl.h"
#include "engine/EngineControl.h"
#include "engine/Services.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include <assert.h>

#include "pt/utility.hpp"

using namespace engine::gl;

ShaderProgram::
ShaderProgram( const pt::Name& name )
{}


ShaderProgram::
~ShaderProgram()
{
    if( 0 != mHandle ){
        gl::DeleteProgram( mHandle );
    }
}


void ShaderProgram::
AddShader( ShaderPtr shader )
{
    if( gl::ShaderType::NO_SHADER_TYPE == shader->GetShaderType() ){
        PT_LOG_ERR( "Tried to add shader'" << shader->GetName().GetStdString()
                    << "' to program '" << mName.GetStdString() << "' with no shader type!" );
        assert( gl::ShaderType::NO_SHADER_TYPE != shader->GetShaderType() );
        return;
    }

    mShaders.reserve( 4 );
    mShaders.push_back( shader );
}


void ShaderProgram::
ClearShaders()
{
    mShaders.clear();
}


void ShaderProgram::
FreeVRAM()
{
    if( 0 != mHandle ){
        PT_LOG_DEBUG( "Freeing up ShaderProgram '" << mName.GetStdString() << "'" );
        gl::DeleteProgram( mHandle );
        mHandle = 0;
    }
}


GLuint ShaderProgram::
GetHandle() const
{
    return mHandle;
}


pt::Name ShaderProgram::
GetName() const
{
    return mName;
}


bool ShaderProgram::
IsLinked() const
{
    return mLinked;
}


bool ShaderProgram::
Link()
{
    PT_LOG_OUT( "Linking ShaderProgram '" << mName.GetStdString() << "'..." );
    pt::log::out << "Shaders(";
    for( auto s : mShaders ){
        pt::log::out << "'" << s->GetName().GetStdString() << "',";
    }
    pt::log::out << ")" << pt::log::send;

    // verify that the program has at least 1 fragment shader
    bool hasFS = false;
    for( auto s : mShaders ){
        if( gl::ShaderType::FRAGMENT_SHADER == s->GetShaderType() ){
            hasFS = true;
            break;
        }
    }
    if( !hasFS ){
        PT_LOG_ERR( "Tried to link shader program '" << mName.GetStdString() << "' without a Fragment Shader attached. Skipping." );
        return false;
    }

    // compile shaders, if needed
    for( auto s : mShaders ){
        bool compiled = s->Compile();
        if( !compiled ){
            return false;
        }
    }

    // create + guard shaderprogram
    if( 0 != mHandle ){
        mHandle = gl::CreateProgram();
    }
    mLinked = false;

    for( auto s : mShaders ){
        gl::AttachShader( mHandle, s->GetHandle() );
    }

    // link the new program
    GLint success = GL_FALSE;
    gl::LinkProgram( mHandle );
    gl::GetProgramiv( mHandle, GL_LINK_STATUS, &success );
    if( GL_FALSE == success ) {
        PT_LOG_ERR( "Error while linking shader program" );
        gl::PrintShaderProgramInfoLog( mHandle );
        return false;
    }
    PT_LOG_DEBUG( "Successfuly linked ShaderProgram '" << mName.GetStdString() << "'" );

    bool devMode = engine::Services::GetEngineControl()->DeveloperMode();
    if( devMode ){
        PT_LOG_OUT( "Validating ShaderProgram'" << mName.GetStdString() << "'" );
        GLint isValidated = GL_FALSE;
        gl::ValidateProgram( mHandle );
        gl::GetProgramiv( mHandle, GL_VALIDATE_STATUS, &isValidated );
        if( GL_FALSE == isValidated ){
            PT_LOG_ERR( "Could not validate ShaderProgram'" << mName.GetStdString() << "'" );
            gl::PrintShaderProgramInfoLog( mHandle );
        }
    }

    mLinked = true;
    return true;
}


void ShaderProgram::
Use()
{
    if( !mLinked ){
        PT_LOG_ERR( "Tried to use non-linked shader program '" << mName.GetStdString() << "'" );
        return;
    }

    assert( 0 != mHandle );
    gl::UseProgram( mHandle );
}

