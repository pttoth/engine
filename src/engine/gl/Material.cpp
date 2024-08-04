#include "engine/gl/Material.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/DrawingControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Utility.h"
#include "pt/logging.h"
#include <assert.h>

using namespace engine;
using namespace engine::gl;


Material::
~Material()
{}


void Material::
Bind()
{
    PT_LOG_LIMITED_WARN( 10, "Normal and Specular maps in Materials are not yet bound to OpenGL!" );
    PT_LOG_LIMITED_WARN( 10, "Reimplement Material::Bind(), to simultaneously bind the diffuse, normal and specular textures!" );

    auto dc = Services::GetDrawingControl();
    if( mTexture0Diffuse ){
        mTexture0Diffuse->BindToTextureUnit( dc->GetMainTextureUnit() );
    }
}


const std::string& Material::
GetName() const
{
    return mName;
}


bool Material::
IsClientSideSynced() const
{
    return !mDirty;
}


void Material::
LoadToVRAM()
{
    // @TODO: load shaders too?

    { gl::Texture2dPtr& currentTex = mTexture0Diffuse;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture0Normal;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture0Specular;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Diffuse;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Normal;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Specular;
    if( currentTex && !currentTex->HasDataInVRAM() ){
        currentTex->LoadToVRAM();
    } }

    mDirty = false;
}


MaterialPtr Material::
CreateFromFile( const std::string& name, const std::string& path )
{
    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Could not open file '" << path << "'" );
        return nullptr;
    }

    std::stringstream ss;
    for ( std::string currentline; std::getline( ifs, currentline ); /* empty */ ){
        ss << currentline << "\n";
    }
    ifs.close();

    MaterialPtr retval = Material::CreateFromString( name, ss.str() );
    if( nullptr == retval ){
        PT_LOG_ERR( "Could not load material file '" << path << "'\n" );
    }else{
        retval->mPath = path;
    }

    return retval;
}


MaterialPtr Material::
CreateFromString( const std::string& name, const std::string& data )
{
    MaterialPtr instance;
    try{
        instance = Material::CreateFromString_ThrowsException( name, data );
    }catch( const std::invalid_argument& e ){
        size_t maxlength = 4096;
        size_t length = std::min( data.length(), maxlength );
        PT_LOG_ERR( "Could not parse config data:\n-----\n'" << data.substr(0, length) << "'\n-----\n  reason: " << e.what() );
    }
    return instance;
}


Material::
Material( const std::string& name ):
    mName( name )
{
    Construct();
}


MaterialPtr Material::
CreateFromString_ThrowsException( const std::string& name, const std::string& data )
{
    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();
    assert( nullptr != ec && nullptr != ac );
    if( nullptr == ec || nullptr == ac ){
        PT_LOG_ERR( "Not all Services are set up, while loading Material!" );
        return nullptr;
    }

    MaterialPtr instance = MaterialPtr( new Material( name ) );

    Material&   mat = *instance.get();
    mat.mCfg.readS( data ); //throws std::invalid_argument
                            // @TODO: don't handle, 'ptlib' will drop exception-errors in later versions

    instance->mTexture0Diffuse    = ac->GetTexture( GetConfigAttribute( mat, strTexture0Diffuse ) );
    instance->mTexture0Normal     = ac->GetTexture( GetConfigAttribute( mat, strTexture0Normal ) );
    instance->mTexture0Specular   = ac->GetTexture( GetConfigAttribute( mat, strTexture0Specular ) );
    instance->mTexture1Diffuse    = ac->GetTexture( GetConfigAttribute( mat, strTexture1Diffuse ) );
    instance->mTexture1Normal     = ac->GetTexture( GetConfigAttribute( mat, strTexture1Normal ) );
    instance->mTexture1Specular   = ac->GetTexture( GetConfigAttribute( mat, strTexture1Specular ) );

    std::string shaderprogramname = GetConfigAttribute( mat, strShaderProgramName );
    if( 0 == shaderprogramname.length() ){
        PT_LOG_WARN( "No shader program found for material '" << instance->GetName() << "'. Using default." );
        instance->mShaderProgram = ac->GetFallbackShaderProgram();
        assert( nullptr != instance->mShaderProgram );
    }else{
        instance->mShaderProgram = ac->GetShaderProgram( shaderprogramname );
        if( nullptr == instance->mShaderProgram ){
            // if doesn't use / couldn't find existing shader program, build custom
            instance->mVertexShader   = ac->GetShader( GetConfigAttribute( mat, strVertexShader ) );
            instance->mGeometryShader = ac->GetShader( GetConfigAttribute( mat, strGeometryShader ) );
            instance->mFragmentShader = ac->GetShader( GetConfigAttribute( mat, strFragmentShader ) );
        }
    }

    instance->mInitialized = true;

    return instance;
}


std::string Material::
GetConfigAttribute( const Material& mat, Attribute key )
{
    try{
        return mat.mCfg.getS( key );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Could not read attribute '" << key << "' from material '" << mat.GetName() << "'" );
    }
    return std::string();
}


void Material::
SetupConfigAttributes( pt::Config& cfg )
{
    CfgAddKey( cfg, strTexture0Diffuse );
    CfgAddKey( cfg, strTexture0Normal );
    CfgAddKey( cfg, strTexture0Specular );
    CfgAddKey( cfg, strTexture1Diffuse );
    CfgAddKey( cfg, strTexture1Normal );
    CfgAddKey( cfg, strTexture1Specular );
    CfgAddKey( cfg, strVertexShader );
    CfgAddKey( cfg, strGeometryShader );
    CfgAddKey( cfg, strFragmentShader );
    CfgAddKey( cfg, strShaderProgramName );
}


void Material::
Construct()
{
    Material::SetupConfigAttributes( mCfg );
}
