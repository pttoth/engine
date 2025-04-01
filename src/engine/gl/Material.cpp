#include "engine/gl/Material.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/service/Renderer.h"
#include "engine/Utility.h"
#include "pt/math.h"
#include "pt/logging.h"
#include <assert.h>

using namespace engine;
using namespace engine::gl;

using namespace math;

void
BindTextureOrFallback( gl::Texture2dPtr& tex, uint32_t texture_unit )
{
    auto ac = Services::GetAssetControl();
    auto r = Services::GetRenderer();

    if( tex ){
        r->BindTextureToUnit( tex, texture_unit );
    }else{
        r->BindTextureToUnit( ac->GetFallbackTexture(), texture_unit );
    }
}


Material::
~Material()
{}


void Material::
Bind()
{
    PT_LOG_LIMITED_WARN( 10, "Normal and Specular maps in Materials are not yet bound to OpenGL!" << "\n"
                          << "    Reimplement Material::Bind(), to simultaneously bind the diffuse, normal and specular textures!" );

    auto r = Services::GetRenderer();

    for( size_t i=0; i<mTextures.size(); ++i ){
        BindTextureOrFallback( mTextures[i], r->GetTextureUnit(i) );
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


bool Material::
IsStub() const
{
    return mIsStub;
}


void Material::
Unbind()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void Material::
LoadToVRAM()
{
    // @TODO: load shaders too?

    for( auto t : mTextures ){
        if( (nullptr != t) && (not t->HasDataInVRAM()) ){
            t->LoadToVRAM();
        }
    }

    mDirty = false;
}


MaterialPtr Material::
CreateFromFile( const std::string& name, const std::string& path )
{
    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Failed to load material '" << name << "'(path: '" << path << "')" );
        return CreateStubMaterial( name );
    }

    std::stringstream ss;
    for ( std::string currentline; std::getline( ifs, currentline ); /* empty */ ){
        ss << currentline << "\n";
    }
    ifs.close();

    MaterialPtr retval = Material::CreateFromString( name, ss.str() );
    retval->mPath = path;

    return retval;
}


MaterialPtr Material::
CreateFromString( const std::string& name, const std::string& data )
{
    MaterialPtr instance;
    try{
        instance = Material::CreateFromString_ThrowsException( name, data );
        if( instance->IsStub() ){
            PT_LOG_ERR( "Created incomplete material '" << name << "'" );
        }
    }catch( const std::invalid_argument& e ){
        size_t maxlength = 4096;
        size_t length = std::min( data.length(), maxlength );
        PT_LOG_ERR( "Could not parse config data:\n-----\n'" << data.substr(0, length) << "'\n-----\n  reason: " << e.what() );
        return CreateStubMaterial( name );
    }

    return instance;
}


MaterialPtr Material::
CreateStubMaterial( const std::string& name )
{
    MaterialPtr instance = MaterialPtr( new Material( name ) );
    instance->mIsStub = true;

    instance->mTextures.resize( stTextureCount );
    for( size_t i=0; i<stTextureCount; ++i ){
        std::stringstream ss;
        ss << name << "." << "tex " << i;
        Texture2dPtr tex =
                Texture2d::CreateFromData_RGBA_FLOAT( ss.str(), int2(16, 16),
                                                      Texture2d::GenerateColorGrid( 16, 16,
                                                                                    vec4( vec3::yellow, 1.0f ),
                                                                                    vec4( vec3::black, 1.0f ) ) );
        tex->SetMinFilter( gl::MinFilter::NEAREST );
        tex->SetMagFilter( gl::MagFilter::NEAREST );
        instance->mTextures[i] = tex;
    }

    auto ac = Services::GetAssetControl();
    instance->mShaderProgram = ac->GetFallbackShaderProgram();

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

    MaterialPtr instance = MaterialPtr( new Material( name ) );
    Material&   mat = *instance.get();
    mat.mCfg.readS( data ); //  throws std::invalid_argument

    // setup texture pointers or 'nullptr' if config val is empty
    instance->mTextures.resize( stTextureCount );
    SetTextureAtIndex( instance, 0, GetConfigAttribute( mat, strTexture0Diffuse ) );
    SetTextureAtIndex( instance, 1, GetConfigAttribute( mat, strTexture0Normal ) );
    SetTextureAtIndex( instance, 2, GetConfigAttribute( mat, strTexture0Specular ) );
    SetTextureAtIndex( instance, 3, GetConfigAttribute( mat, strTexture1Diffuse ) );
    SetTextureAtIndex( instance, 4, GetConfigAttribute( mat, strTexture1Normal ) );
    SetTextureAtIndex( instance, 5, GetConfigAttribute( mat, strTexture1Specular ) );

    std::string shaderprogramname = GetConfigAttribute( mat, strShaderProgramName );
    if( 0 == shaderprogramname.length() ){
        PT_LOG_DEBUG( "No shader program found for material '" << instance->GetName() << "'. Using default." );
        instance->mShaderProgram = ac->GetFallbackShaderProgram();
        instance->mIsStub = true;
    }else{
        instance->mShaderProgram = ac->GetShaderProgram( shaderprogramname );
    }

    instance->mInitialized = true;

    // check if created material holds any stubs
    if( !instance->IsStub()
        || instance->HasStubTextures()
        || instance->mShaderProgram->IsStub() )
    {
        instance->mIsStub  = true;
    }

//  This function does not have access to 'path' and is private
//  so delegate error printing to the caller function (this won't be called from outside the class)
//    if( instance->IsStub() ){
//        PT_LOG_ERR( "Failed to load material '" << name << "'" );
//    }

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

    CfgAddKey( cfg, strShaderProgramName );
}


uint32_t Material::
GetTextureIndex( uint32_t slot, TexComponent texcomponent )
{
    assert( slot < stTextureSlotCount );
    return 3*slot + texcomponent;
}


void Material::
SetTextureAtIndex( MaterialPtr mat, uint32_t idx, const std::string& name )
{
    assert( idx < stTextureCount );
    auto ac = Services::GetAssetControl();
    Texture2dPtr tex = nullptr;
    if( 0 != name.length() ){
        tex = ac->GetTexture( name );
    }
    mat->mTextures[idx] = tex;
}


bool Material::
HasStubTextures() const
{
    for( auto t : mTextures ){
        if( (nullptr != t) && t->IsStub() ){
            return true;
        }
    }

    return false;
}


void Material::
Construct()
{
    Material::SetupConfigAttributes( mCfg );
}

