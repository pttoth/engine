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


bool AssetManager::
PrefetchTexture( const std::string& path )
{
    return FetchTexture( path );
}


gl::MaterialPtr AssetManager::
GetMaterial( const std::string& path )
{
    if( 0 == path.length() ){
        return nullptr;
    }
    gl::MaterialPtr material = mMaterials[path]; // map alloc happens here, if doesn't exist
    if( nullptr == material ){
        PT_LOG_WARN( "Late-fetching material '" << path << "'" );
        material = NewPtr<gl::Material>();
        mMaterials[path] = material;
        material->ReadFile( path );
    }
    return material;
}


MeshPtr AssetManager::
GetMesh( const std::string& name )
{
    if( 0 < mMeshes.count( name ) ){
        return mMeshes[name];
    }

    PT_LOG_DEBUG( "First-time request for mesh '" << name << "'" );
    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    MeshPtr mesh = NewPtr<Mesh>( name );
    mMeshes[name] = mesh;
    /*mesh->ReadFile( ec->ResolveMediaFilePath(
                        ac->ResolveMeshFileName( name ) ) );
    */
    mesh->ReadFile( name );

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
        bool suc = FetchTexture( name );
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
    if( 0 == mShaders.count( name ) ){
        return nullptr;
    }
    gl::ShaderPtr retval = mShaders[name];
#ifdef PT_DEBUG_ENABLED
    if( nullptr == retval ){
        PT_LOG_WARN( "Leftover 'nullptr' entry in AssetManager::mShaders for shadername '" << name << "'!" );
    }
#endif
    return retval;
}


gl::ShaderProgramPtr AssetManager::
GetShaderProgram( const pt::Name& name )
{
    if( 0 == name.length() ){
        return nullptr;
    }
    if( 0 == mShaderPrograms.count( name ) ){
        return nullptr;
    }
    gl::ShaderProgramPtr retval = mShaderPrograms[name];
#ifdef PT_DEBUG_ENABLED
    if( nullptr == retval ){
        PT_LOG_WARN( "Leftover 'nullptr' entry in AssetManager::mShaderPrograms for shaderprogram name '" << name << "'!" );
    }
#endif
    return retval;
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


bool AssetManager::
FetchTexture( const std::string& path )
{
    if( 0 == path.length() ){
        PT_LOG_ERR( "Tried to read empty path as PNG file!");
        return false;
    }
    if( 0 < mTextures.count( path ) ){
        return true;
    }

    auto ec = Services::GetEngineControl();

    gl::Texture2dPtr texture = NewPtr<gl::Texture2d>( path );
    mTextures[path] = texture;
    texture->ReadFilePNG( path );
    return texture->IsLoadedInRAM();
}
