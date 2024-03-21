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
ShaderProgram( const pt::Name& name ):
    mName( name )
{}


ShaderProgram::
~ShaderProgram()
{
    if( 0 != mHandle ){
        gl::DeleteProgram( mHandle );
    }
}


ShaderProgram::
ShaderProgram( ShaderProgram&& source ):
    mDirty( source.mDirty ), mLinked( source.mLinked ), mName( std::move(source.mName) ),
    mShaders( std::move(source.mShaders) ), mHandle( source.mHandle )
{
    source.SetDefaultMemberValues();
}


ShaderProgram& ShaderProgram::
operator=( ShaderProgram&& source )
{
    if( this != &source ){
        FreeVRAM();

        mDirty   = source.mDirty;
        mLinked  = source.mLinked;
        mName    = std::move( source.mName );
        mShaders = std::move( source.mShaders );
        mHandle  = source.mHandle;

        source.SetDefaultMemberValues();
    }
    return *this;
}


void ShaderProgram::
AddShader( ShaderPtr shader )
{
    if( gl::ShaderType::NO_SHADER_TYPE == shader->GetShaderType() ){
        PT_LOG_ERR( "Tried to add shader'" << shader->GetName()
                    << "' to program '" << mName << "' with no shader type!" );
        assert( gl::ShaderType::NO_SHADER_TYPE != shader->GetShaderType() );
        return;
    }

    mShaders.reserve( 3 );
    mShaders.push_back( shader );
    mDirty = true;
}


void ShaderProgram::
AddUniformName( const pt::Name& name )
{
    name.Init();
    int64_t idx = pt::IndexOfInVector( mUniformNames, name );
    assert( idx < 0 );
    if( -1 < idx ){
        PT_LOG_ERR( "Tried to add the same Uniform name '" << name << "' multiple times to shader program '" << GetName() << "'" );
        return;
    }
    mUniformNames.push_back( name );
}


void ShaderProgram::
ClearShaders()
{
    mShaders.clear();
    mDirty = true;
}


void ShaderProgram::
FreeVRAM()
{
    if( 0 != mHandle ){
        PT_LOG_DEBUG( "Freeing up ShaderProgram '" << mName << "'" );
        gl::DeleteProgram( mHandle );
        mHandle = 0;
        mLinked = false;
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

const std::vector<pt::Name>& ShaderProgram::
GetUniformNames() const
{
    return mUniformNames;
}


bool ShaderProgram::
IsLinked() const
{
    return mLinked;
}


bool ShaderProgram::
Link()
{
    if( !mDirty ){
        return true;
    }

    PT_LOG_OUT( "Linking ShaderProgram '" << mName << "'..." );
    {
        std::ostringstream oss;
        for( auto s : mShaders ){
            oss << "'" << s->GetName() << "',";
        }
        PT_LOG_OUT( "Shaders(" << oss.str() << ")" );
    }

    // verify that the program has at least 1 fragment shader
    bool hasFS = false;
    for( auto s : mShaders ){
        if( gl::ShaderType::FRAGMENT_SHADER == s->GetShaderType() ){
            hasFS = true;
            break;
        }
    }
    if( !hasFS ){
        PT_LOG_ERR( "Tried to link shader program '" << mName << "' without a Fragment Shader attached. Skipping." );
        return false;
    }

    // compile shaders, if needed
    for( auto s : mShaders ){
        bool compiled = s->Compile();
        if( !compiled ){
            return false;
        }
    }

    // create shaderprogram
    if( 0 == mHandle ){
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
        gl::PrintProgramInfoLog( mHandle );
        return false;
    }
    PT_LOG_DEBUG( "Successfuly linked ShaderProgram(" << mHandle << ")'" << mName << "'" );

    bool devMode = engine::Services::GetEngineControl()->DeveloperMode();
    if( devMode ){
        PT_LOG_OUT( "Validating ShaderProgram(" << mHandle << ")'" << mName << "'" );
        GLint isValidated = GL_FALSE;
        gl::ValidateProgram( mHandle );
        gl::GetProgramiv( mHandle, GL_VALIDATE_STATUS, &isValidated );
        if( GL_FALSE == isValidated ){
            PT_LOG_ERR( "Could not validate ShaderProgram'" << mName << "'" );
            gl::PrintProgramInfoLog( mHandle );
            return false;
        }
        PT_LOG_DEBUG( "ShaderProgram(" << mHandle << ")'" << mName << "' is valid" );
    }

    mDirty = false;
    mLinked = true;
    OnLinked();

    return true;
}


void ShaderProgram::
Use()
{
    if( !mLinked ){
        PT_LOG_ERR( "Tried to use non-linked shader program '" << mName << "'" );
        return;
    }
    if( mDirty ){
        PT_LOG_WARN( "Shader program '" << mName << "' currently in use was modified since last link. Forgot re-link?" );
    }

    assert( 0 < mHandle );
    gl::UseProgram( mHandle );
}


void ShaderProgram::
OnLinked()
{}
