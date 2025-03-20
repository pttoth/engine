#include "engine/gl/ShaderProgram.h"

#include "engine/gl/GlWrapper.h"
#include "engine/service/EngineControl.h"
#include "engine/service/Renderer.h"
#include "engine/Services.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include <assert.h>

#include "pt/utility.hpp"

using namespace engine::gl;



//ShaderProgramPtr ShaderProgram::
//CreateFromBinaryFile( const std::string& name, const std::string& path )
//{}


ShaderProgramPtr ShaderProgram::
CreateFromDescriptorFile( const std::string& name, const std::string& path )
{
    PT_LOG_DEBUG( "Creating ShaderProgram '" << name << "' from file '" << path << "'" );

    pt::Config cfg;
    CfgAddKey( cfg, strVertexShader );
    CfgAddKey( cfg, strGeometryShader );
    CfgAddKey( cfg, strFragmentShader );

    ShaderProgramPtr instance = NewPtr<ShaderProgram>( name );
    instance->mPath = path;

    try{
        cfg.readF( path );

        AddShadersFromConfig( instance, cfg );
    }catch( std::invalid_argument& e ){
        PT_LOG_ERR( "Error while reading shader file '" << path << "'\n"
                    "    reason: \"" << e.what() << "\"" );
        instance->mShaders.clear();
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        ShaderPtr fsh = Shader::CreateStubShader( ShaderType::FRAGMENT_SHADER );
        instance->mShaders.push_back( fsh );
    }

    return instance;
}


ShaderProgramPtr ShaderProgram::
CreateFromString( const std::string& name, const std::string& data )
{
    PT_LOG_DEBUG( "Creating ShaderProgram '" << name << "' from string" );

    pt::Config cfg;
    CfgAddKey( cfg, strVertexShader );
    CfgAddKey( cfg, strGeometryShader );
    CfgAddKey( cfg, strFragmentShader );

    ShaderProgramPtr instance = NewPtr<ShaderProgram>( name );

    try{
        cfg.readS( data );

        AddShadersFromConfig( instance, cfg );
    }catch( std::invalid_argument& e ){
        PT_LOG_ERR( "Error while reading shader descriptor string\n"
                    "    reason: \"" << e.what() << "\"\n"
                    "    data: \n\"" << data << "\"" );
        instance->mShaders.clear();
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        ShaderPtr fsh = Shader::CreateStubShader( ShaderType::FRAGMENT_SHADER );
        instance->mShaders.push_back( fsh );
    }

    return instance;
}


ShaderProgramPtr ShaderProgram::
CreateFromShaderList( const std::string& name, const std::vector<ShaderPtr>& shaders )
{
#ifdef PT_DEBUG_ENABLED
    std::stringstream ss;
    for( auto s : shaders ){
        ss << "'";
        if( nullptr != s ){
            ss << s->GetName();
        }
        ss << "'\n";
    }
    PT_LOG_DEBUG( "Creating ShaderProgram '" << name << "' from shader list:\n"
                  << ss.str() );
#endif

    ShaderProgramPtr instance = NewPtr<ShaderProgram>( name );

    // if data is invalid, create a stub
    if( ( 0 == shaders.size() )
        || ( nullptr == shaders[0] )
        || ( ShaderType::VERTEX_SHADER != shaders[0]->GetShaderType() ) )
    {
        PT_LOG_DEBUG( "Tried to create ShaderProgram '" << name << "' from invalid data!" );
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        ShaderPtr fsh = Shader::CreateStubShader( ShaderType::FRAGMENT_SHADER );
        instance->mShaders.push_back( fsh );

    }else{
        // otherwise, create shaderprogram from data
        for( auto s : shaders ){
            if( nullptr == s ){
                PT_LOG_ERR( "'nullptr' among Shaders while creating ShaderProgram '" << name << "'" );
                continue;
            }
            instance->mShaders.push_back( s );
        }
    }

    return instance;
}


ShaderProgram::
ShaderProgram( const std::string& name ):
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
AddUniformName( const std::string& name )
{
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


const std::string& ShaderProgram::
GetName() const
{
    return mName;
}


const std::string&
ShaderProgram::GetPath() const
{
    return mPath;
}


std::vector<std::string> ShaderProgram::
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
        PT_LOG_LIMITED_ERR( 50, "Tried to use non-linked shader program '" << mName << "'" );
        return;
    }
    if( mDirty ){
        PT_LOG_LIMITED_WARN( 50, "Shader program '" << mName << "' currently in use was modified since last link. Forgot re-link?" );
    }

    assert( 0 < mHandle );
    gl::UseProgram( mHandle );
}


void ShaderProgram::
OnLinked()
{}


void ShaderProgram::
AddShadersFromConfig( ShaderProgramPtr shaderprog, const pt::Config& config )
{
    {
        const std::string& sh_path = config.getS( strVertexShader );
        if( 0 < sh_path.length() ){
            ShaderPtr sh = Shader::CreateFromFile( sh_path, ShaderType::VERTEX_SHADER, sh_path );
            shaderprog->mShaders.push_back( sh );
        }else{
            PT_LOG_ERR( "ShaderProgram '" << shaderprog->mName << "' does not have a vertex shader!" );
            ShaderPtr sh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
            shaderprog->mShaders.push_back( sh );
        }
    }
    {
        const std::string& sh_path = config.getS( strGeometryShader );
        if( 0 < sh_path.length() ){
            ShaderPtr sh = Shader::CreateFromFile( sh_path, ShaderType::GEOMETRY_SHADER, sh_path );
            shaderprog->mShaders.push_back( sh );
        }
    }
    {
        const std::string& sh_path = config.getS( strFragmentShader );
        if( 0 < sh_path.length() ){
            ShaderPtr sh = Shader::CreateFromFile( sh_path, ShaderType::FRAGMENT_SHADER, sh_path );
            shaderprog->mShaders.push_back( sh );
        }
    }
}
