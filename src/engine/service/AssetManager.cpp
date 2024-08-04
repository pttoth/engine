#include "engine/service/AssetManager.h"

#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Services.h"

#include "pt/logging.h"
#include "pt/utility.hpp"



using namespace engine;

AssetManager::
AssetManager()
{}


void AssetManager::
SafeReleaseMesh( const std::string& name )
{
    //TODO: implement

    // this is currently commented out to avoid spam
    //PT_WARN_UNIMPLEMENTED_FUNCTION
}


gl::MaterialPtr AssetManager::
GetFallbackMaterial()
{
    return mFallbackMaterial;
}


gl::MeshPtr AssetManager::
GetFallbackMesh()
{
    PT_UNIMPLEMENTED_FUNCTION
    return nullptr;
}


gl::Texture2dPtr AssetManager::
GetFallbackTexture()
{
    PT_UNIMPLEMENTED_FUNCTION
    return nullptr;
}


gl::ShaderProgramPtr AssetManager::
GetFallbackShaderProgram()
{
    return mFallbackShaderProgram;
}


AssetManager::
~AssetManager()
{}


gl::MaterialPtr AssetManager::
GetMaterial( const std::string& name )
{
    if( 0 == name.length() ){
        const char* errmsg = "Tried to fetch empty name as material!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        return nullptr;
    }

    // find out, whether it's already stored
    gl::MaterialPtr mat = FindMaterial( name );
    if( nullptr != mat ){
        return mat;
    }

    // late-fetch material
    PT_LOG_WARN( "Late-fetching material '" << name << "'" );
    bool suc = LoadMaterial( name );
    if( suc ){
        mat = FindMaterial( name );
        if( nullptr != mat ){
            return mat;
        }
    }

    // use fallback instead
    PT_LOG_ERR( "Asset Manager could not retrieve material '" << name << "'. Returning fallback instead." );
    return GetFallbackMaterial();
}


gl::MeshPtr AssetManager::
GetMesh( const std::string& name )
{
    if( 0 == name.length() ){
        const char* errmsg = "Tried to fetch empty name as mesh!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        return nullptr;
    }

    auto iter = mMeshes.find( name );
    if( mMeshes.end() != iter ){
        return iter->second;
    }

    PT_LOG_WARN( "Late-fetching mesh '" << name << "'" );
    bool suc = LoadMesh( name );
    if( suc ){
        iter = mMeshes.find( name );
        if( mMeshes.end() != iter ){
            return iter->second;
        }
    }

    PT_LOG_ERR( "Asset Manager could not retrieve mesh '" << name << "'. Returning fallback instead." );
    return GetFallbackMesh();
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
        /*
        const char* errmsg = "Requested empty path as texture asset from AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        */
        return nullptr;
    }

    auto iter = mTextures.find( name );
    if( mTextures.end() != iter ){
        return iter->second;
    }

    PT_LOG_WARN( "Late-fetching texture '" << name << "'" );
    bool suc = LoadTexture( name );
    if( suc ){
        iter = mTextures.find( name );
        if( mTextures.end() != iter ){
            return iter->second;
        }
    }

    PT_LOG_ERR( "Asset Manager could not retrieve texture '" << name << "'. Returning fallback instead." );
    return GetFallbackTexture();
}


gl::ShaderPtr AssetManager::
GetShader( const pt::Name& name )
{
    //TODO: rewrite

    if( 0 == name.length() ){
        /*
        const char* errmsg = "Requested empty path as shader asset from AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        */
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
    //TODO: rewrite

    if( 0 == name.length() ){
        /*
        const char* errmsg = "Requested empty path as shaderprogram asset from AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        */
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
LoadMaterial( const std::string& name )
{
    if( 0 == name.length() ){
        const char* errmsg = "Tried to read empty path as material in AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        return false;
    }
    if( 0 < mMaterials.count( name ) ){
        return true;
    }

    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();
    gl::MaterialPtr material = gl::Material::CreateFromFile( name, ec->ResolveMediaFilePath(
                                                                     ac->ResolveMaterialFileName( name ) ) );

    if( nullptr != material ){
        mMaterials[name] = material;
        return true;
    }

    PT_LOG_ERR( "Failed to load material '" << name << "'" );
    return false;
}


bool AssetManager::
LoadMesh( const std::string& name, gl::Mesh::FormatHint hint )
{
    if( 0 == name.length() ){
        const char* errmsg = "Tried to load empty name as mesh in AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        return false;
    }

    // check if mesh is loaded
    if( 0 < mMeshes.count( name ) ){
        return true;
    }

    // try loading mesh
    //auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    gl::MeshPtr mesh = gl::Mesh::CreateFromFile( name, hint );

    if( nullptr != mesh ){
        mMeshes[name] = mesh;
        return true;
    }

    PT_LOG_ERR( "Failed to load mesh '" << name << "'" );
    return false;
}


bool AssetManager::
LoadTexture( const std::string& name )
{
    if( 0 == name.length() ){
        const char* errmsg = "Tried to read empty path as PNG file in AssetManager!";
        PT_LOG_ERR( errmsg );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace( errmsg );
        #endif
        return false;
    }
    if( 0 < mTextures.count( name ) ){
        return true;
    }

    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();

    gl::Texture2dPtr texture = gl::Texture2d::CreateFromPNG( name, ec->ResolveMediaFilePath(
                                                                     ac->ResolveTextureFileName( name ) ) );
    mTextures[name] = texture;

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
ResolveMeshFileName( const std::string& name, gl::Mesh::FormatHint hint )
{
    switch( hint ){
    case gl::Mesh::FormatHint::MD5_IDTECH4:
        return name + ".md5mesh";
        break;
    case gl::Mesh::FormatHint::GLTF:
        return name + ".gltf";
        break;
    }
    return name;
}


std::string AssetManager::
ResolveTextureFileName( const std::string& name )
{
    return name; // temporary override, until texture format is not finalized
    return name + ".png";
}


bool AssetManager::
SetFallbackMaterial( gl::MaterialPtr material )
{
    bool suc = AddMaterial( material );
    if( suc ){
        mFallbackMaterial = material;
    }
    return suc;
}


bool AssetManager::
SetFallbackShaderProgram( gl::ShaderProgramPtr shaderprogram )
{
    bool suc = AddShaderProgram( shaderprogram );
    if( suc ){
        mFallbackShaderProgram = shaderprogram;
    }
    return suc;
}


bool AssetManager::
AddMaterial( gl::MaterialPtr material )
{
    if( nullptr == material ){
        PT_LOG_ERR( "Tried to add 'nullptr' as material to AssetManager!" );
        return false;
    }

    const std::string& name = material->GetName();

    const gl::MaterialPtr mat = this->FindMaterial( name );
    if( nullptr != mat ){
        if( mat.get() != material.get() ){
            PT_LOG_WARN( "Detected multiple, different Materials with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same Material '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return false;
    }
    mMaterials[name] = material;
    return true;
}


void AssetManager::
RemoveMaterial( const pt::Name& name )
{
    PT_UNIMPLEMENTED_FUNCTION
}


bool AssetManager::
AddShader( gl::ShaderPtr shader )
{
    if( nullptr == shader ){
        PT_LOG_WARN( "Tried to add 'nullptr' as Shader to AssetManager!" );
        return false;
    }

    const std::string& name = shader->GetName().GetStdString();

    if( 0 < mShaders.count( name ) ){
        if( shader.get() != mShaders[name].get() ){
            PT_LOG_WARN( "Detected multiple, different Shaders with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same Shader '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return false;
    }
    mShaders[name] = shader;
    return true;
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


bool AssetManager::
AddShaderProgram( gl::ShaderProgramPtr shaderprogram )
{
    if( nullptr == shaderprogram ){
        PT_LOG_WARN( "Tried to add 'nullptr' as ShaderProgram to AssetManager!" );
        return false;
    }

    const std::string& name = shaderprogram->GetName().GetStdString();

    if( 0 < mShaderPrograms.count( name ) ){
        if( shaderprogram.get() != mShaderPrograms[name].get() ){
            PT_LOG_WARN( "Detected multiple, different ShaderPrograms with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same ShaderProgram '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return false;
    }
    mShaderPrograms[name] = shaderprogram;
    return true;
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
AddTexture( gl::Texture2dPtr texture )
{
    if( nullptr == texture ){
        PT_LOG_WARN( "Tried to add 'nullptr' as Texture to AssetManager!" );
        return false;
    }

    const std::string& name = texture->GetName().GetStdString();

    // @TODO: rewrite with 'FindTexture()' later...
    if( 0 < mTextures.count( name ) ){
        if( texture.get() != mTextures[name].get() ){
            PT_LOG_WARN( "Detected multiple, different Textures with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same Texture '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return false;
    }
    mTextures[name] = texture;
    return true;
}


void AssetManager::
RemoveTexture( const pt::Name& name )
{
    PT_UNIMPLEMENTED_FUNCTION
}


gl::MaterialPtr AssetManager::
FindMaterial( const std::string& name ) const
{
    auto iter = mMaterials.find( name );
    if( mMaterials.end() != iter ){
        return iter->second;
    }
    return nullptr;
}

