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
        //@TODO: get this from AssetManager
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        //@TODO: get this from AssetManager
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
        //@TODO: get this from AssetManager
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        //@TODO: get this from AssetManager
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
        //@TODO: get this from AssetManager
        ShaderPtr vsh = Shader::CreateStubShader( ShaderType::VERTEX_SHADER );
        instance->mShaders.push_back( vsh );
        //@TODO: get this from AssetManager
        ShaderPtr fsh = Shader::CreateStubShader( ShaderType::FRAGMENT_SHADER );
        instance->mShaders.push_back( fsh );

    }else{
        // otherwise, create shaderprogram from data
        for( auto s : shaders ){
            if( (nullptr == s)
                || (ShaderType::NO_SHADER_TYPE == s->GetShaderType()) )
            {
                PT_LOG_ERR( "Encountered invalid data among Shaders while creating ShaderProgram '" << name << "'" );
                continue;
            }
            instance->mShaders.push_back( s );
        }
    }

    return instance;
}


ShaderProgram::
~ShaderProgram()
{
    if( 0 != mHandle ){
        gl::DeleteProgram( mHandle );
    }
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


const std::string& ShaderProgram::
GetPath() const
{
    return mPath;
}


bool ShaderProgram::
IsLinked() const
{
    return mLinked;
}


bool ShaderProgram::
Link()
{
    if( 0 != mHandle ){
        FreeVRAM();
    }

    PT_LOG_OUT( "Linking ShaderProgram '" << mName << "'" );
    {
        std::ostringstream oss;
        for( auto s : mShaders ){
            oss << "'";
            if( nullptr != s ){
                oss << s->GetName();
            }
            oss << "',";
        }

        PT_LOG_DEBUG( "  Shaders( " << oss.str() << " )" );
    }

    // compile shaders, if needed
    for( auto s : mShaders ){
        if( !s->IsCompiled() ){
            bool compiled = s->Compile();
            if( !compiled ){
                return false;
            }
        }
    }


    mHandle = gl::CreateProgram();
    auto guard = pt::CreateGuard( [this]{
        gl::DeleteProgram( mHandle );
    } );

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

    mLinked = true;
    OnLinked();
    guard.Disable();

    return true;
}


void ShaderProgram::
Use()
{
    if( !mLinked ){
        PT_LOG_LIMITED_ERR( 50, "Tried to use non-linked shader program '" << mName << "'" );
        return;
    }

    assert( 0 < mHandle );
    gl::UseProgram( mHandle );
}


ShaderProgram::
ShaderProgram( const std::string& name ):
    mName( name )
{}


void ShaderProgram::
OnLinked()
{}


void ShaderProgram::
AddShadersFromConfig( ShaderProgramPtr shaderprog, const pt::Config& config )
{
    shaderprog->mShaders.reserve( 2 );
    {
        const std::string& sh_path = config.getS( strVertexShader );
        if( 0 < sh_path.length() ){
            //@TODO: get this from AssetManager
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
            //@TODO: get this from AssetManager
            ShaderPtr sh = Shader::CreateFromFile( sh_path, ShaderType::GEOMETRY_SHADER, sh_path );
            shaderprog->mShaders.push_back( sh );
        }
    }
    {
        const std::string& sh_path = config.getS( strFragmentShader );
        if( 0 < sh_path.length() ){
            //@TODO: get this from AssetManager
            ShaderPtr sh = Shader::CreateFromFile( sh_path, ShaderType::FRAGMENT_SHADER, sh_path );
            shaderprog->mShaders.push_back( sh );
        }
    }
}
