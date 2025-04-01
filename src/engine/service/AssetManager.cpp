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
    PT_LOG_LIMITED_WARN( 50, "'AssetManager::SafeReleaseMesh()' is unimplemented!" );
}


gl::MaterialPtr AssetManager::
GetFallbackMaterial()
{
    assert( nullptr != mFallbackMaterial );
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
    assert( nullptr != mFallbackTexture );
    return mFallbackTexture;
}


gl::ShaderProgramPtr AssetManager::
GetFallbackShaderProgram()
{
    assert( nullptr != mFallbackShaderProgram );
    return mFallbackShaderProgram;
}


AssetManager::
~AssetManager()
{}


gl::MaterialPtr AssetManager::
GetMaterial( const std::string& name )
{
    if( 0 == name.length() ){
        return GetFallbackMaterial();
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
    PT_LOG_ERR( "Asset Manager could not retrieve material '" << name << "'." );
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
        return GetFallbackMesh();
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

    PT_LOG_ERR( "Asset Manager could not retrieve mesh '" << name << "'." );
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
        return GetFallbackTexture();
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

    PT_LOG_ERR( "Asset Manager could not retrieve texture '" << name << "'." );
    return GetFallbackTexture();
}


gl::ShaderPtr AssetManager::
GetShader( const std::string& name )
{
    // @TODO: rewrite
    //  add late-fetch

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
GetShaderProgram( const std::string& name )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid query for shaderprogram in asset manager" );
        return GetFallbackShaderProgram();
    }

    // search for 'name' and if found, return it
    auto iter = mShaderPrograms.find( name );
    if( mShaderPrograms.end() != iter ){
        return iter->second;
    }

    PT_LOG_WARN( "Late-fetching shaderprogram '" << name << "'" );
    LoadShaderProgram( name );
    iter = mShaderPrograms.find( name );
    return iter->second;    // no error check needed
}


bool AssetManager::
LoadMaterial( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid load request for material in asset manager" );
        return false;
    }

    // skip load, if found and reload is not forced
    if( (!force) && (0 < mMaterials.count( name )) ){
        return true;
    }

    auto ec = Services::GetEngineControl();

    std::string     path     = ec->ResolveMediaFilePath(
                                this->ResolveMaterialFileName( name ) );
    gl::MaterialPtr instance = gl::Material::CreateFromFile( name, path );


    //-----
    //@TODO: remove after stub creation is implemented
    //PT_WARN_UNIMPLEMENTED_FUNCTION
    if( nullptr != instance ){
        mMaterials[name] = instance;
        PT_LOG_DEBUG( "Loaded material '" << name << "'" );
        return true;
    }
    //-----


    mMaterials[name] = instance;

    bool success = !instance->IsStub();
    if( !success ){
        PT_LOG_ERR( "Failed to load material '" << name << "'(path: '" << path << "')" );
    }
    return success;
}


bool AssetManager::
LoadMesh( const std::string& name, gl::Mesh::FormatHint hint, bool force )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid load request for mesh in asset manager" );
        return false;
    }

    // skip load, if found and reload is not forced
    if( (!force) && (0 < mMeshes.count( name )) ){
        return true;
    }

    //-----
    // @TODO: refactor !!!!
    PT_WARN_UNIMPLEMENTED_FUNCTION
    gl::MeshPtr instance = gl::Mesh::CreateFromFile( name, hint );
    if( nullptr != instance ){
        mMeshes[name] = instance;
        return true;
    }

    std::string path;
    //-----

    mMeshes[name] = instance;

    bool success = !instance->IsStub();
    if( !success ){
        PT_LOG_ERR( "Failed to load mesh '" << name << "'(path: '" << path << "')" );
    }
    return success;
}


bool AssetManager::
LoadShader( const std::string& name, gl::ShaderType type_, bool force )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid load request for shader in asset manager" );
        return false;
    }

    if( (!force) && (0 < mShaders.count( name )) ){
        return true;
    }

    auto ec = Services::GetEngineControl();

    gl::ShaderType final_type = type_;

    // if no shader type supplied, deduce it from file extension
    if( gl::ShaderType::NO_SHADER_TYPE == type_ ){
        std::string ext = pt::StringPostfix( name, 3 );
        if( ".fs" == ext ){
            final_type = gl::ShaderType::FRAGMENT_SHADER;
        }else if( ".vs" == ext ){
            final_type = gl::ShaderType::VERTEX_SHADER;
        }else if( ".gs" == ext ){
            final_type = gl::ShaderType::GEOMETRY_SHADER;
        }else if( false ){
            // @TODO: add other shader extensions
        }else{
            PT_LOG_ERR( "Failed to load shader '" << name << "'!\n  Unknown shader type!" );
            return false;
        }
    }

    std::string path = ec->ResolveMediaFilePath( name );
    gl::ShaderPtr instance = gl::Shader::CreateFromFile( name, final_type, path );

    mShaders[name] = instance;

    bool success = !instance->IsStub();
    if( !success ){
        PT_LOG_ERR( "Failed to load shader '" << name << "'(path: '" << path << "')" );
    }
    return success;
}


bool AssetManager::
LoadShaderProgram( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid load request for shaderprogram in asset manager" );
        return false;
    }

    // skip load, if found and reload is not forced
    if( (!force) && (0 < mShaderPrograms.count( name )) ){
        return true;
    }

    auto ec = Services::GetEngineControl();

    std::string path = ec->ResolveMediaFilePath( name );
    gl::ShaderProgramPtr instance = gl::ShaderProgram::CreateFromDescriptorFile( name, path );

    mShaderPrograms[name] = instance;

    bool success = !instance->IsStub();
    if( !success ){
        PT_LOG_ERR( "Failed to load shaderprogram '" << name << "'(path: '" << path << "')" );
    }
    return success;
}


bool AssetManager::
LoadTexture( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid load request for texture in asset manager" );
        return false;
    }

    // skip load, if found and reload is not forced
    if( (!force) && (0 < mTextures.count( name )) ){
        return true;
    }

    auto ec = Services::GetEngineControl();

    std::string path = ec->ResolveMediaFilePath(
                        this->ResolveTextureFileName( name ) );
    gl::Texture2dPtr instance = gl::Texture2d::CreateFromPNG( name, path );

    //-----
    //@TODO: remove after stub creation is implemented
    PT_WARN_UNIMPLEMENTED_FUNCTION
    if( nullptr == instance ){
        return false;
    }
    //-----

    mTextures[name] = instance;

    bool success = !instance->IsStub();
    if( !success ){
        PT_LOG_ERR( "Failed to load texture '" << name << "'(path: '" << path << "')" );
    }
    return success;
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
ResolveShaderFileName( const std::string& name )
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
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
SetFallbackMaterialTexture( gl::Texture2dPtr texture )
{
    bool suc = AddTexture( texture );
    if( suc ){
        mFallbackMaterialTexture = texture;
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
SetFallbackTexture( gl::Texture2dPtr texture )
{
    bool suc = AddTexture( texture );
    if( suc ){
        mFallbackTexture = texture;
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
RemoveMaterial( const std::string& name )
{
    PT_UNIMPLEMENTED_FUNCTION
}


bool AssetManager::
AddMesh( gl::MeshPtr mesh )
{
    if( nullptr == mesh ){
        PT_LOG_WARN( "Tried to add 'nullptr' as Mesh to AssetManager!" );
        return false;
    }

    const std::string& name = mesh->GetName();

    if( 0 < mMeshes.count( name ) ){
        if( mesh.get() != mMeshes[name].get() ){
            PT_LOG_WARN( "Detected multiple, different Meshes with the same name '" << name << "', while trying to add them to AssetManager! Skipping add." );
        }else{
            PT_LOG_DEBUG( "Tried to add the same Mesh '" << name << "' multiple times to AssetManager! Skipping add." );
        }
        return false;
    }
    mMeshes[name] = mesh;
    return true;
}


void AssetManager::
RemoveMesh( const std::string& name )
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

    const std::string& name = shader->GetName();

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
RemoveShader( const std::string& name )
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

    const std::string& name = shaderprogram->GetName();

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
RemoveShaderProgram( const std::string& name )
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

    const std::string& name = texture->GetName();

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
RemoveTexture( const std::string& name )
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

