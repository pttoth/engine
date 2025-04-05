#include "engine/gl/ShaderProgram.h"

#include "engine/gl/GlWrapper.h"
#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/service/Renderer.h"
#include "engine/Services.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include <assert.h>



using namespace engine::gl;



enum ShaderProgramAttribute{
    strVertexShader,
    strGeometryShader,
    strFragmentShader,
};


void
SetupConfig( pt::Config& cfg )
{
    CfgAddKey( cfg, strVertexShader );
    CfgAddKey( cfg, strGeometryShader );
    CfgAddKey( cfg, strFragmentShader );
}


std::string
GetShaderListAsString( const std::vector<ShaderPtr>& list, bool multiline )
{
    std::stringstream ss;
    bool first = true;

    if( !multiline ){
        ss << "(";
        for( auto s : list ){
            if( !first ){
                ss << ", ";
                first = false;
            }
            ss << "'";
            if( nullptr != s ){
                ss << s->GetName();
            }
            ss << "'";
        }
        ss << ")";
    }else{  // if multiline
        ss << "(";
        for( auto s : list ){
            if( !first ){
                ss << "\n";
                first = false;
            }
            ss << "    '";
            if( nullptr != s ){
                ss << s->GetName();
            }
            ss << "'";
        }
        ss << ")";
    }

    return ss.str();
}



//ShaderProgramPtr ShaderProgram::
//CreateFromBinaryFile( const std::string& name, const std::string& path )
//{}



ShaderProgramPtr ShaderProgram::
CreateFromDescriptorFile( const std::string& name, const std::string& path )
{
    PT_LOG_DEBUG( "Creating ShaderProgram '" << name << "' from file '" << path << "'" );
    return CreateFromData( name, path, true );
}


ShaderProgramPtr ShaderProgram::
CreateFromString( const std::string& name, const std::string& data )
{
    PT_LOG_DEBUG( "Creating ShaderProgram '" << name << "' from string:\n"
                  << "'" << data << "'" );
    return CreateFromData( name, data, false );
}


ShaderProgramPtr ShaderProgram::
CreateFromShaderList( const std::string& name, const std::vector<ShaderPtr>& shaders )
{
    ShaderProgramPtr instance = ShaderProgramPtr( new ShaderProgram( name ) );

    bool hasBadShaders = false;
    for( auto s : shaders ){
        if( (nullptr == s) || (ShaderType::NO_SHADER_TYPE == s->GetShaderType()) ){
            hasBadShaders = true;
            break;
        }
    }

    // if data is invalid, create a stub
    if( ( 0 == shaders.size() )
        || ( nullptr == shaders[0] )
        || ( ShaderType::VERTEX_SHADER != shaders[0]->GetShaderType() )
        || hasBadShaders )
    {
        SetupInstanceAsStub( instance );
        PT_LOG_ERR( "Failed to create ShaderProgram '" << name << "'. Invalid list of shaders.\n"
                    << GetShaderListAsString( shaders, false ));
        return instance;
    }

    // input data is valid, create shaderprogram from data
    for( auto s : shaders ){
        instance->mShaders.push_back( s );
    }

    return instance;
}


ShaderProgram::
~ShaderProgram()
{
    FreeVRAM();
}


ShaderProgram::
ShaderProgram( ShaderProgram&& source ):
    ev_OnLinked( evTrigger_OnLinked ),
    mLinked( source.mLinked ),
    mName( std::move( source.mName ) ),
    mPath( std::move( source.mPath ) ),
    mShaders( std::move( source.mShaders ) ),
    mHandle( source.mHandle ),
    evTrigger_OnLinked( std::move(source.evTrigger_OnLinked) )
{
    source.mLinked = false;
    source.mHandle = 0;
}


ShaderProgram& ShaderProgram::
operator=( ShaderProgram&& source )
{
    mLinked  = source.mLinked;
    mName    = std::move( source.mName );
    mPath    = std::move( source.mPath );
    mShaders = std::move( source.mShaders );
    mHandle  = source.mHandle;
    evTrigger_OnLinked = std::move( source.evTrigger_OnLinked );

    //source.mLinked = false;
    //source.mHandle = 0;
    return *this;
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
IsStub() const
{
    for( auto s : mShaders ){
        if( s->IsStub() ){
            return true;
        }
    }
    return false;
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
    evTrigger_OnLinked();
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
    ev_OnLinked( evTrigger_OnLinked ), mName( name )
{}


void ShaderProgram::
AddShadersFromConfig( ShaderProgramPtr shaderprog, const pt::Config& config )
{
    auto ac = Services::GetAssetControl();
    assert( nullptr != ac );

    shaderprog->mShaders.reserve( 2 );
    {
        ShaderPtr sh;
        const std::string& sh_name = config.getS( strVertexShader );
        if( 0 < sh_name.length() ){
            ac->LoadShader( sh_name, gl::ShaderType::VERTEX_SHADER );       // @TODO: (very low prio): remove | prevents some late-fetch warnings
            sh = ac->GetShader( sh_name, gl::ShaderType::VERTEX_SHADER );
        }else{
            PT_LOG_ERR( "ShaderProgram '" << shaderprog->mName << "' does not have a vertex shader!" );
            sh = ac->GetFallbackShader( gl::ShaderType::VERTEX_SHADER );
        }
        shaderprog->mShaders.push_back( sh );
    }
    {
        const std::string& sh_name = config.getS( strGeometryShader );
        if( 0 < sh_name.length() ){
            ac->LoadShader( sh_name, gl::ShaderType::GEOMETRY_SHADER );     // @TODO: (very low prio): remove | prevents some late-fetch warnings
            ShaderPtr sh = ac->GetShader( sh_name, gl::ShaderType::GEOMETRY_SHADER );
            shaderprog->mShaders.push_back( sh );
        }
    }
    {
        const std::string& sh_name = config.getS( strFragmentShader );
        if( 0 < sh_name.length() ){
            ac->LoadShader( sh_name, gl::ShaderType::FRAGMENT_SHADER );     // @TODO: (very low prio): remove | prevents some late-fetch warnings
            ShaderPtr sh = ac->GetShader( sh_name, gl::ShaderType::FRAGMENT_SHADER );
            shaderprog->mShaders.push_back( sh );
        }
    }
}


ShaderProgramPtr ShaderProgram::
CreateFromData( const std::string& name, const std::string& data, bool data_is_path )
{
    pt::Config cfg;
    SetupConfig( cfg );

    ShaderProgramPtr instance = ShaderProgramPtr( new ShaderProgram( name ) );

    try{
        if( data_is_path ){
            instance->mPath = data;
            cfg.readF( data );
        }else{
            cfg.readS( data );
        }

        AddShadersFromConfig( instance, cfg );
    }catch( std::invalid_argument& e ){
        if( data_is_path ){
            PT_LOG_ERR( "Error while reading shader file '" << data << "'\n"
                        "    reason: \"" << e.what() << "\"" );
        }else{
            PT_LOG_ERR( "Error while reading shader descriptor string\n"
                        "    reason: \"" << e.what() << "\"\n"
                        "    data: \n\"" << data << "\"" );
        }
        SetupInstanceAsStub( instance );
        PT_LOG_ERR( "Failed to create ShaderProgram '" << name << "'" );
    }

    return instance;
}


void ShaderProgram::
SetupInstanceAsStub( ShaderProgramPtr instance )
{
    assert( nullptr != instance );
    if( nullptr == instance ){
        return;
    }

    auto ac = Services::GetAssetControl();
    assert( nullptr != ac );
    instance->mShaders.clear();
    instance->mShaders.reserve(2);
    instance->mShaders.push_back( ac->GetFallbackShader( ShaderType::VERTEX_SHADER ) );
    instance->mShaders.push_back( ac->GetFallbackShader( ShaderType::FRAGMENT_SHADER ) );
}
