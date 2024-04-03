#include "engine/gl/Material.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
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
GetAttribute( const pt::Config& cfg, Attribute key, const std::string& id )
{
    try{
        return cfg.getS( key );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Could not read attribute '" << key << "' from material '" << id << "'" );
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
    mTexture0Diffuse->Bind();
}


void Material::
Clear()
{
    if( mInitialized ){
        mInitialized = true;
        mCfg.clear();
        mPath.clear();

        //TODO:
        //mType =

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


bool Material::
IsClientSideSynced() const
{
    return !mDirty;
}


void Material::
ReadFile( const std::string& path )
{
    Clear();
    try{
        mCfg.readF( path );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Error while reading file '" << path << "'\n  reason:" << e.what() );
        return;
    }

    auto ac = Services::GetAssetControl();
    if( nullptr == ac ){
        return;
    }

    mPath               = path;
    mTexture0Diffuse    = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture0Diffuse, path ) ) );
    mTexture0Normal     = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture0Normal, path ) ) );
    mTexture0Specular   = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture0Specular, path ) ) );
    mTexture1Diffuse    = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture1Diffuse, path ) ) );
    mTexture1Normal     = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture1Normal, path ) ) );
    mTexture1Specular   = ac->GetTexture( ResolveMediaFilePath( GetAttribute( mCfg, strTexture1Specular, path ) ) );

    std::string shaderprogramname = GetAttribute( mCfg, strShaderProgramName, path );
    if( 0 != shaderprogramname.length() ){
        mShaderProgram = ac->GetShaderProgram( shaderprogramname );
        if( mShaderProgram ){
            return;
        }
        PT_LOG_ERR( "Could not load shader program '" << shaderprogramname << "' for material '" << path << "'" );
    }

    // if doesn't use / couldn't find existing shader program, build custom
    mVertexShader       = ac->GetShader( ResolveMediaFilePath( GetAttribute( mCfg, strVertexShader, path ) ) );
    mGeometryShader     = ac->GetShader( ResolveMediaFilePath( GetAttribute( mCfg, strGeometryShader, path ) ) );
    mFragmentShader     = ac->GetShader( ResolveMediaFilePath( GetAttribute( mCfg, strFragmentShader, path ) ) );

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
