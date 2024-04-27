#include "engine/gl/Material.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Utility.h"
#include "pt/logging.h"
#include <assert.h>

using namespace engine;
using namespace engine::gl;

enum Attribute{
    strTexture0Diffuse,
    strTexture0Normal,
    strTexture0Specular,
    strTexture1Diffuse,
    strTexture1Normal,
    strTexture1Specular,
    strVertexShader,
    strGeometryShader,
    strFragmentShader,
    strShaderProgramName
};


void
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


// Helper function to simplify per-call try-catch logic
std::string
GetAttribute( const Material& mat, const pt::Config& cfg, Attribute key )
{
    try{
        return cfg.getS( key );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Could not read attribute '" << key << "' from material '" << mat.GetName() << "'" );
    }
    return std::string();
}


//---------------------------------------------------------------------------

Material::
Material()
{
    Construct();
}


Material::
Material( const std::string& name ):
    mName( name )
{
    Construct();
}


Material::
~Material()
{}


Material::
Material( Material&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


Material& Material::
operator=( Material&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


void Material::
Bind()
{
    PT_LOG_LIMITED_WARN( 10, "Normal and Specular maps in Materials are not yet bound to OpenGL!" );
    PT_LOG_LIMITED_WARN( 10, "Reimplement Material::Bind(), to simultaneously bind the diffuse, normal and specular textures!" );
    if( mTexture0Diffuse ){
        mTexture0Diffuse->Bind();
    }
}


void Material::
Clear()
{
    if( mInitialized ){
        mInitialized = false;
        mCfg.clear();
        mPath.clear();

        //TODO:
        //mType =

        //TODO: these textures shouldn't be default 'nullptr' rather default purp-black fallback texture
        mTexture0Diffuse    = nullptr;
        mTexture0Normal     = nullptr;
        mTexture0Specular   = nullptr;
        mTexture1Diffuse    = nullptr;
        mTexture1Normal     = nullptr;
        mTexture1Specular   = nullptr;
        mVertexShader       = nullptr;
        mGeometryShader     = nullptr;
        mFragmentShader     = nullptr;
        mShaderProgram      = nullptr;
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
ReadFile( const std::string& path )
{
    Clear();
    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();
    assert( nullptr != ec && nullptr != ac );
    if( nullptr == ec || nullptr == ac ){
        return;
    }

    try{
        mCfg.readF( path );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Error while reading file '" << path << "'\n  reason: " << e.what() );
        return;
    }

    mPath               = path;
    mTexture0Diffuse    = ac->GetTexture( GetAttribute( *this, mCfg, strTexture0Diffuse ) );
    mTexture0Normal     = ac->GetTexture( GetAttribute( *this, mCfg, strTexture0Normal ) );
    mTexture0Specular   = ac->GetTexture( GetAttribute( *this, mCfg, strTexture0Specular ) );
    mTexture1Diffuse    = ac->GetTexture( GetAttribute( *this, mCfg, strTexture1Diffuse ) );
    mTexture1Normal     = ac->GetTexture( GetAttribute( *this, mCfg, strTexture1Normal ) );
    mTexture1Specular   = ac->GetTexture( GetAttribute( *this, mCfg, strTexture1Specular ) );

    std::string shaderprogramname = GetAttribute( *this, mCfg, strShaderProgramName );
    if( 0 != shaderprogramname.length() ){
        mShaderProgram = ac->GetShaderProgram( shaderprogramname );
        if( mShaderProgram ){
            return;
        }
        PT_LOG_ERR( "Could not load shader program '" << shaderprogramname << "' for material '" << mName << "'" );
    }

    // if doesn't use / couldn't find existing shader program, build custom
    mVertexShader       = ac->GetShader( GetAttribute( *this, mCfg, strVertexShader ) );
    mGeometryShader     = ac->GetShader( GetAttribute( *this, mCfg, strGeometryShader ) );
    mFragmentShader     = ac->GetShader( GetAttribute( *this, mCfg, strFragmentShader ) );

    mInitialized = true;
}


void Material::
LoadToVRAM()
{
    { gl::Texture2dPtr& currentTex = mTexture0Diffuse;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture0Normal;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture0Specular;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Diffuse;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Normal;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }
    { gl::Texture2dPtr& currentTex = mTexture1Specular;
    if( currentTex && !currentTex->IsLoadedInVRAM() ){
        currentTex->LoadToVRAM();
    } }

    mDirty = false;
}


void Material::
Construct()
{
    SetupConfigAttributes( mCfg );
}
