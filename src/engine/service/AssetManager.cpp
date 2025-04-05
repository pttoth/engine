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


gl::ShaderPtr AssetManager::
GetFallbackShader( gl::ShaderType type )
{
    gl::ShaderPtr retval = mFallbackShaders[type];
    assert( nullptr != retval);
    return retval;
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
    // search for 'name' and if found, return it
    auto iter = mMaterials.find( name );
    if( mMaterials.end() != iter ){
        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' material found under name '" << name << "' in Asset Manager! Removing." );
            mMaterials.erase( iter );
        }else{
            return iter->second;
        }
    }

    // late-fetch material
    PT_LOG_WARN( "Late-fetching material '" << name << "'" );
    LoadMaterial( name );
    return mMaterials.find( name )->second;
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
    // search for 'name' and if found, return it
    auto iter = mTextures.find( name );
    if( mTextures.end() != iter ){
        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' texture found under name '" << name << "' in Asset Manager! Removing." );
            mTextures.erase( iter );
        }else{
            return iter->second;
        }
    }

    // late-fetch texture
    PT_LOG_WARN( "Late-fetching texture '" << name << "'" );
    LoadTexture( name );
    return mTextures.find( name )->second;
}


gl::ShaderPtr AssetManager::
GetShader( const std::string& name )
{
    gl::ShaderType type = GuessShaderTypeByName( name );
    if( gl::ShaderType::NO_SHADER_TYPE == type ){
        PT_LOG_INFO( "Failed to guess shader type from name '" << name << "'" );
    }else{
        PT_LOG_INFO( "Guessing shader type '" << gl::GetShaderTypeAsString(type) << "'from name '" << name << "'" );
    }

    return GetShader( name, type );
}


gl::ShaderPtr AssetManager::
GetShader( const std::string& name, gl::ShaderType type )
{
    if( gl::ShaderType::NO_SHADER_TYPE == type ){
        PT_LOG_LIMITED_ERR( 10, "Tried to request shader '" << name << "' without type from asset manager" );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Tried to request shader '" + name + "' without type from asset manager" );
        return nullptr;
    }

    // search for 'name' and if found, return it
    auto iter = mShaders.find( name );
    if( mShaders.end() != iter ){
        if( iter->second->GetShaderType() != type ){
            std::stringstream ss;
            ss << "Shader type mismatch between request and stored shader with name '" << name << "'";
            PT_LOG_LIMITED_ERR( 10, ss.str() );
            PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, ss.str() );
            return GetFallbackShader( type );
        }

        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' shader found under name '" << name << "' in Asset Manager! Removing." );
            mShaders.erase( iter );
        }else{
            return iter->second;
        }
    }

    PT_LOG_WARN( "Late-fetching shaderprogram '" << name << "'" );
    bool success = LoadShader( name, type );
    if( success ){
        return mShaders.find( name )->second;
    }else{
        return GetFallbackShader( type );   // case: no-type shader
    }
}


gl::ShaderProgramPtr AssetManager::
GetShaderProgram( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Invalid query for shaderprogram in asset manager" );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Invalid query for shaderprogram in asset manager" );
        return GetFallbackShaderProgram();
    }

    // search for 'name' and if found, return it
    auto iter = mShaderPrograms.find( name );
    if( mShaderPrograms.end() != iter ){
        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' shaderprogram found under name '" << name << "' in Asset Manager! Removing." );
            mShaderPrograms.erase( iter );
        }else{
            return iter->second;
        }
    }

    PT_LOG_WARN( "Late-fetching shaderprogram '" << name << "'" );
    LoadShaderProgram( name );
    return mShaderPrograms.find( name )->second;
}


bool AssetManager::
LoadMaterial( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Loading material with no name in asset manager." );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Loading material with no name in asset manager." );
    }

    // check if already contained
    auto iter = mMaterials.find( name );
    if( iter != mMaterials.end() ){
        gl::MaterialPtr mat = iter->second;
        assert( nullptr != mat );
        if( nullptr == mat ){
            PT_LOG_ERR( "Stray 'nullptr' material found under name '" << name << "' in Asset Manager! Removing." );
            mMaterials.erase( iter );
        }else{
            if( !force ){
                return !mat->IsStub();
            }
        }
    }

    auto ec = Services::GetEngineControl();
    assert( nullptr != ec );

    gl::MaterialPtr instance = gl::Material::CreateFromFile( name,
                                                             ec->ResolveMediaFilePath(
                                                                 this->ResolveMaterialFileName( name ) ) );
    mMaterials[name] = instance;

    return !instance->IsStub();
}


bool AssetManager::
LoadMesh( const std::string& name, gl::Mesh::FormatHint hint, bool force )
{
    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Invalid load request for mesh in asset manager" );
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
LoadShader( const std::string& name )
{
    gl::ShaderType type = GuessShaderTypeByName( name );
    if( gl::ShaderType::NO_SHADER_TYPE == type ){
        PT_LOG_INFO( "Failed to guess shader type from name '" << name << "'" );
    }else{
        PT_LOG_INFO( "Guessing shader type '" << gl::GetShaderTypeAsString(type) << "'from name '" << name << "'" );
    }

    return LoadShader( name, type );
}


bool AssetManager::
LoadShader( const std::string& name, gl::ShaderType type, bool force )
{
    if( gl::ShaderType::NO_SHADER_TYPE == type ){
        PT_LOG_ERR( "Tried to load shader '" << name << "' with no type." );
        return false;
    }

    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Loading shader with no name in asset manager." );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Loading shader with no name in asset manager." );
    }

    // check if already contained
    auto iter = mShaders.find( name );
    if( iter != mShaders.end() ){
        gl::ShaderPtr sh = iter->second;
        assert( nullptr != sh );
        if( nullptr == sh ){
            PT_LOG_ERR( "Stray 'nullptr' shader found under name '" << name << "' in Asset Manager! Removing." );
            mShaders.erase( iter );
        }else{
            if( !force ){
                // if already contained, but with a different type
                if( type != sh->GetShaderType() ){
                    PT_LOG_ERR( "Tried to load shader '" << name << "' as type '" << gl::GetShaderTypeAsString(type)
                                 << "' while one with the same name is already loaded as '" <<  gl::GetShaderTypeAsString(sh->GetShaderType()) << "'." );
                    return false;
                }
                return !sh->IsStub();
            }
        }
    }

    auto ec = Services::GetEngineControl();
    assert( nullptr != ec );
    std::string path = ec->ResolveMediaFilePath( name );
    gl::ShaderPtr instance = gl::Shader::CreateFromFile( name, type, path );
    // no verification needed here, because 'type' was guaranteed to be a valid one by now

    mShaders[name] = instance;

    return !instance->IsStub();
}


bool AssetManager::
LoadShaderProgram( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Loading shaderprogram with no name in asset manager." );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Loading shaderprogram with no name in asset manager." );
    }

    // check if already contained
    auto iter = mShaderPrograms.find( name );
    if( iter != mShaderPrograms.end() ){
        gl::ShaderProgramPtr shp = iter->second;
        assert( nullptr != shp );
        if( nullptr == shp ){
            PT_LOG_ERR( "Stray 'nullptr' shaderprogram found under name '" << name << "' in Asset Manager! Removing." );
            mShaderPrograms.erase( iter );
        }else{
            if( !force ){
                return !shp->IsStub();
            }
        }
    }

    auto ec = Services::GetEngineControl();
    assert( nullptr != ec );
    std::string path = ec->ResolveMediaFilePath( name );
    gl::ShaderProgramPtr instance = gl::ShaderProgram::CreateFromDescriptorFile( name, path );

    mShaderPrograms[name] = instance;

    return !instance->IsStub();
}


bool AssetManager::
LoadTexture( const std::string& name, bool force )
{
    if( 0 == name.length() ){
        PT_LOG_LIMITED_ERR( 10, "Loading texture with no name in asset manager." );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Loading texture with no name in asset manager." );
    }

    // check if already contained
    auto iter = mTextures.find( name );
    if( iter != mTextures.end() ){
        gl::Texture2dPtr tex = iter->second;
        assert( nullptr != tex );
        if( nullptr == tex ){
            PT_LOG_ERR( "Stray 'nullptr' texture found under name '" << name << "' in Asset Manager! Removing." );
            mTextures.erase( iter );
        }else{
            if( !force ){
                return !tex->IsStub();
            }
        }
    }

    auto ec = Services::GetEngineControl();
    assert( nullptr != ec );

    gl::Texture2dPtr instance = gl::Texture2d::CreateFromPNG( name,
                                                              ec->ResolveMediaFilePath(
                                                                  this->ResolveTextureFileName( name ) ) );
    mTextures[name] = instance;

    return !instance->IsStub();
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


void AssetManager::
SetFallbackMaterial( gl::MaterialPtr material )
{
    mFallbackMaterial = material;
}


void AssetManager::
SetFallbackMaterialTexture( gl::Texture2dPtr texture )
{
    mFallbackMaterialTexture = texture;
}


void AssetManager::
SetFallbackShader( gl::ShaderPtr shader, gl::ShaderType type  )
{
    mFallbackShaders[type] = shader;
}


void AssetManager::
SetFallbackShaderProgram( gl::ShaderProgramPtr shaderprogram )
{
    mFallbackShaderProgram = shaderprogram;
}


void AssetManager::
SetFallbackTexture( gl::Texture2dPtr texture )
{
    mFallbackTexture = texture;
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


gl::ShaderType AssetManager::
GuessShaderTypeByName( const std::string& name )
{
    std::string ext = pt::StringPostfix( name, 3 );
    if( ".fs" == ext ){
        return gl::ShaderType::FRAGMENT_SHADER;
    }else if( ".vs" == ext ){
        return gl::ShaderType::VERTEX_SHADER;
    }else if( ".gs" == ext ){
        return gl::ShaderType::GEOMETRY_SHADER;
    }
    // @TODO: add other shader extensions

    return gl::ShaderType::NO_SHADER_TYPE;
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

