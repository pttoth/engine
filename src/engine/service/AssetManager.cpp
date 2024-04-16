#include "engine/service/AssetManager.h"

#include "engine/EngineControl.h"
#include "engine/service/AssetControl.h"
#include "engine/Services.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

using namespace engine;

AssetManager::
AssetManager()
{}


AssetManager::
~AssetManager()
{}


gl::MaterialPtr AssetManager::
GetMaterial( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_WARN( "Tried to fetch empty name as material!" );
        return nullptr;
    }

    auto iter = mMaterials.find( name );
    if( mMaterials.end() != iter ){
        return iter->second;
    }

    if( !mLateFetchEnabled ){
        return nullptr;
    }

    PT_LOG_WARN( "Late-fetching material '" << name << "'" );
    gl::MaterialPtr material = NewPtr<gl::Material>( name );
    mMaterials[name] = material;

    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    material->ReadFile( ec->ResolveMediaFilePath(
                          ac->ResolveMaterialFileName( name ) ) );

    return material;
}


MeshPtr AssetManager::
GetMesh( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_WARN( "Tried to fetch empty name as mesh!" );
        return nullptr;
    }

    auto iter = mMeshes.find( name );
    if( mMeshes.end() != iter ){
        return iter->second;
    }

    if( !mLateFetchEnabled ){
        return nullptr;
    }

    PT_LOG_WARN( "Late-fetching mesh '" << name << "'" );
    MeshPtr mesh = NewPtr<Mesh>( name );
    mMeshes[name] = mesh;

    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    mesh->ReadFile( ec->ResolveMediaFilePath(
                      ac->ResolveMeshFileName( name ) ) );
    return mesh;
}


MeshLoaderPtr AssetManager::
GetMeshLoader()
{
    // TODO: check currently executing thread id
    //   [create +] return meshloader associated with given thread
    if( nullptr == mMeshLoader ){
        mMeshLoader = NewPtr<MeshLoader>();
    }
    return mMeshLoader;
}


gl::Texture2dPtr AssetManager::
GetTexture( const std::string& name )
{
    if( 0 == name.length() ){
        return nullptr;
    }
    if( 0 == mTextures.count( name ) ){
        PT_LOG_WARN( "Late-fetching texture '" << name << "'" );
        bool suc = LoadTexture( name );
        if( !suc ){
            return nullptr;
        }
    }
    return mTextures[name];
}


gl::ShaderPtr AssetManager::
GetShader( const pt::Name& name )
{
    if( 0 == name.length() ){
        return nullptr;
    }
    auto iter = mShaders.find( name );
    if( mShaders.end() == iter ){
        PT_LOG_ERR( "Missing requested shader resource '" << name << "' in AssetManager!" );
        return nullptr;
    }

#ifdef PT_DEBUG_ENABLED
    if( nullptr == iter->second ){
        PT_LOG_ERR( "Leftover 'nullptr' entry in AssetManager::mShaders for shadername '" << name << "'!" );
    }
#endif
    return iter->second;
}


gl::ShaderProgramPtr AssetManager::
GetShaderProgram( const pt::Name& name )
{
    if( 0 == name.length() ){
        return nullptr;
    }
    auto iter = mShaderPrograms.find( name );
    if( mShaderPrograms.end() == iter ){
        PT_LOG_ERR( "Missing requested shader program resource '" << name << "' in AssetManager!" );
        return nullptr;
    }

#ifdef PT_DEBUG_ENABLED
    if( nullptr == iter->second ){
        PT_LOG_WARN( "Leftover 'nullptr' entry in AssetManager::mShaderPrograms for shaderprogram name '" << name << "'!" );
    }
#endif
    return iter->second;
}


bool AssetManager::
LoadTexture( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Tried to read empty path as PNG file!");
        return false;
    }
    if( 0 < mTextures.count( name ) ){
        return true;
    }

    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    gl::Texture2dPtr texture = NewPtr<gl::Texture2d>( name );
    mTextures[name] = texture;
    texture->ReadFilePNG( ec->ResolveMediaFilePath(
                            ac->ResolveTextureFileName( name ) ) );
    return true;
}


std::string AssetManager::
ResolveAssimpConfigFileName( const std::string& name )
{
    return name + ".assimp.cfg";
}


std::string AssetManager::
ResolveMaterialFileName( const std::string& name )
{
    return name + ".mtr";
}


std::string AssetManager::
ResolveMeshAdapterFileName( const std::string& name )
{
    return name + ".adapter.txt";
}


std::string AssetManager::
ResolveMeshFileName( const std::string& name )
{
    return name + ".md5mesh";
}


std::string AssetManager::
ResolveTextureFileName( const std::string& name )
{
    return name; // temporary override, until texture format is not finalized
    return name + ".png";
}


void AssetManager::
AddShader( gl::ShaderPtr shader )
{
    const std::string& name = shader->GetName().GetStdString();
    if( nullptr == shader ){
        PT_LOG_WARN( "Tried to add 'nullptr' as Shader to AssetManager!" );
        return;
    }

    if( 0 < mShaders.count( name ) ){
        if( shader.get() != mShaders[name].get() ){
            PT_LOG_WARN( "Detected multiple, different Shaders with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same Shader '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return;
    }
    mShaders[name] = shader;
}


void AssetManager::
RemoveShader( const pt::Name& name )
{
    if( 0 == mShaders.count( name ) ){
        PT_LOG_WARN( "Tried to remove a non-contained Shader '" << name << "' from AssetManager." );
        return;
    }
    mShaders.erase( name );
}


void AssetManager::
AddShaderProgram( gl::ShaderProgramPtr shaderprogram )
{
    const std::string& name = shaderprogram->GetName().GetStdString();
    if( nullptr == shaderprogram ){
        PT_LOG_WARN( "Tried to add 'nullptr' as ShaderProgram to AssetManager!" );
        return;
    }

    if( 0 < mShaderPrograms.count( name ) ){
        if( shaderprogram.get() != mShaderPrograms[name].get() ){
            PT_LOG_WARN( "Detected multiple, different ShaderPrograms with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same ShaderProgram '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return;
    }
    mShaderPrograms[name] = shaderprogram;
}


void AssetManager::
RemoveShaderProgram( const pt::Name& name )
{
    if( 0 == mShaderPrograms.count( name ) ){
        PT_LOG_WARN( "Tried to remove a non-contained ShaderProgram '" << name << "' from AssetManager." );
        return;
    }
    mShaderPrograms.erase( name );
}

