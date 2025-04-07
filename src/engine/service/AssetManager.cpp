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
    // search for 'name' and if found, return it
    auto iter = mMeshes.find( name );
    if( mMeshes.end() != iter ){
        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' mesh found under name '" << name << "' in Asset Manager! Removing." );
            mMeshes.erase( iter );
        }else{
            return iter->second;
        }
    }

    // late-fetch material
    PT_LOG_WARN( "Late-fetching material '" << name << "'" );
    LoadMesh( name );
    return mMeshes.find( name )->second;
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


//gl::ShaderPtr AssetManager::
//GetShader( const std::string& name )
//{
//    gl::ShaderType type = GuessShaderTypeByName( name );
//    if( gl::ShaderType::NO_SHADER_TYPE == type ){
//        PT_LOG_INFO( "Failed to guess shader type from name '" << name << "'" );
//    }else{
//        PT_LOG_INFO( "Guessing shader type '" << gl::GetShaderTypeAsString(type) << "'from name '" << name << "'" );
//    }

//    return GetShader( name, type );
//}


gl::ShaderPtr AssetManager::
GetShader( const std::string& name, gl::ShaderType type )
{
    // search for 'name' and if found, return it
    auto iter = mShaders.find( name );
    if( mShaders.end() != iter ){
        if( nullptr == iter->second ){
            PT_LOG_ERR( "Stray 'nullptr' shader found under name '" << name << "' in Asset Manager! Removing." );
            mShaders.erase( iter );
        }else{
            gl::ShaderType stored_type = iter->second->GetShaderType();
            if( gl::ShaderType::NO_SHADER_TYPE == stored_type ){
                PT_LOG_ERR( "Invalid (no-type) shader found under name '" << name << "' in Asset Manager! Removing." );
                mShaders.erase( iter );
            }else{
                if( (stored_type != type) && (gl::ShaderType::NO_SHADER_TYPE != type) ){
                    std::stringstream ss;
                    ss << "Shader type mismatch between request('" << gl::GetShaderTypeAsString(type)
                       << "') and stored('" << gl::GetShaderTypeAsString(stored_type) << "') shader with name '" << name << "'";
                    PT_LOG_LIMITED_ERR( 10, ss.str() );
                    PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, ss.str() );
                    return GetFallbackShader( type );
                }else{
                    return iter->second;
                }
            }
        }
    }

    gl::ShaderType guessed_type = type;
    if( gl::ShaderType::NO_SHADER_TYPE == type ){
        guessed_type = GuessShaderTypeByName( name );
    }

    assert( gl::ShaderType::NO_SHADER_TYPE != guessed_type );
    if( gl::ShaderType::NO_SHADER_TYPE == guessed_type ){
        PT_LOG_LIMITED_ERR( 10, "Tried to request shader '" << name << "' without type from asset manager" );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Tried to request shader '" + name + "' without type from asset manager" );
        return nullptr;
    }

    PT_LOG_WARN( "Late-fetching shaderprogram '" << name << "'" );
    bool success = LoadShader( name, guessed_type );
    if( success ){
        return mShaders.find( name )->second;
    }else{
        return GetFallbackShader( guessed_type );   // case: no-type shader
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
        PT_LOG_LIMITED_ERR( 10, "Loading mesh with no name in asset manager." );
        PT_PRINT_DEBUG_STACKTRACE_LIMITED( 10, "Loading mesh with no name in asset manager." );
    }

    // check if already contained
    auto iter = mMeshes.find( name );
    if( iter != mMeshes.end() ){
        gl::MeshPtr mesh = iter->second;
        assert( nullptr != mesh );
        if( nullptr == mesh ){
            PT_LOG_ERR( "Stray 'nullptr' mesh found under name '" << name << "' in Asset Manager! Removing." );
            mMeshes.erase( iter );
        }else{
            if( !force ){
                return !mesh->IsStub();
            }
        }
    }


    auto ec = Services::GetEngineControl();

    std::string meshfilename = ec->ResolveMediaFilePath(
                                    this->ResolveMeshFileName( name, hint ) );
// @TODO: use this (need '/media' restructure first)
//    gl::MeshPtr instance = gl::Mesh::CreateFromFile( name,
//                                                     meshfilename,
//                                                     ec->ResolveMediaFilePath(
//                                                         this->ResolveAssimpConfigFileName( meshfilename ) ),
//                                                     ec->ResolveMediaFilePath(
//                                                         this->ResolveMeshAdapterFileName( meshfilename ) ) );
    gl::MeshPtr instance = gl::Mesh::CreateFromFile( name,
                                                     meshfilename,
                                                     ec->ResolveMediaFilePath(
                                                         this->ResolveAssimpConfigFileName( name ) ),
                                                     ec->ResolveMediaFilePath(
                                                         this->ResolveMeshAdapterFileName( name ) ) );

    mMeshes[name] = instance;

    return !instance->IsStub();
}


bool AssetManager::
LoadShader( const std::string& name, bool force )
{
    return LoadShader( name, GuessShaderTypeByName( name ), force );
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
    std::string path = ec->ResolveMediaFilePath( this->ResolveShaderFileName( name, type ) );
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
ResolveShaderFileName( const std::string& name, gl::ShaderType type )
{
    gl::ShaderType guessed_type = GuessShaderTypeByName( name );
    // if type was supplied, but also detected in name, don't append twice
    if( guessed_type != type ){
        // use 'type' as basis, except when it's notype, then use 'guessed_type'
        gl::ShaderType actual_type = (gl::ShaderType::NO_SHADER_TYPE == type) ? guessed_type : type;
        switch( actual_type ){
        case gl::ShaderType::VERTEX_SHADER:                 return name + ".vs";
        case gl::ShaderType::GEOMETRY_SHADER:               return name + ".gs";
        case gl::ShaderType::FRAGMENT_SHADER:               return name + ".fs";
        case gl::ShaderType::TESS_CONTROL_SHADER:
            assert( false );
            return name + ".tcs";
        case gl::ShaderType::TESS_EVALUATION_SHADER:
            assert( false );
            return name + ".tes";
        case gl::ShaderType::COMPUTE_SHADER:
            assert( false );
            return name + ".comps";
        default:
            return name;
        }
    }

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
SetFallbackMesh( gl::MeshPtr mesh )
{
    mFallbackMesh = mesh;
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
AddMaterial( gl::MaterialPtr material, bool force )
{
    if( nullptr == material ){
        PT_LOG_ERR( "Tried to add 'nullptr' as material to AssetManager!" );
        return false;
    }

    const std::string& name = material->GetName();
    if( !force ){
        auto iter = mMaterials.find( name );
        if( mMaterials.end() != iter ){
            if( iter->second != nullptr ){
                PT_LOG_ERR( "Failed to add material '" << material->GetName() << "' to Asset Manager! A material with this name is already contained." );
                return false;
            }
        }
    }

    mMaterials[name] = material;
    return true;
}


bool AssetManager::
AddMesh( gl::MeshPtr mesh, bool force )
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


bool AssetManager::
AddShader( gl::ShaderPtr shader, bool force )
{
    if( nullptr == shader ){
        PT_LOG_ERR( "Tried to add 'nullptr' as shader to AssetManager!" );
        return false;
    }

    const std::string& name = shader->GetName();
    if( !force ){
        auto iter = mShaders.find( name );
        if( mShaders.end() != iter ){
            if( iter->second != nullptr ){
                PT_LOG_ERR( "Failed to add shader '" << shader->GetName() << "' to Asset Manager! A shader with this name is already contained." );
                return false;
            }
        }
    }

    mShaders[name] = shader;
    return true;
}


bool AssetManager::
AddShaderProgram( gl::ShaderProgramPtr shaderprogram, bool force )
{
    if( nullptr == shaderprogram ){
        PT_LOG_ERR( "Tried to add 'nullptr' as shaderprogram to AssetManager!" );
        return false;
    }

    const std::string& name = shaderprogram->GetName();
    if( !force ){
        auto iter = mShaderPrograms.find( name );
        if( mShaderPrograms.end() != iter ){
            if( nullptr != iter->second ){
                PT_LOG_ERR( "Failed to add shaderprogram '" << shaderprogram->GetName() << "' to Asset Manager! A shaderprogram with this name is already contained." );
                return false;
            }
        }
    }

    mShaderPrograms[name] = shaderprogram;
    return true;
}


bool AssetManager::
AddTexture( gl::Texture2dPtr texture, bool force )
{
    if( nullptr == texture ){
        PT_LOG_ERR( "Tried to add 'nullptr' as texture to AssetManager!" );
        return false;
    }

    const std::string& name = texture->GetName();
    if( !force ){
        auto iter = mMaterials.find( name );
        if( mMaterials.end() != iter ){
            if( nullptr != iter->second ){
                PT_LOG_ERR( "Failed to add texture '" << texture->GetName() << "' to Asset Manager! A texture with this name is already contained." );
                return false;
            }
        }
    }

    mTextures[name] = texture;
    return true;
}


gl::ShaderType AssetManager::
GuessShaderTypeByName( const std::string& name, bool silent )
{
    std::string ext = pt::StringPostfix( name, 3 );
    gl::ShaderType type;
    if( ".fs" == ext ){
        type = gl::ShaderType::FRAGMENT_SHADER;
    }else if( ".vs" == ext ){
        type = gl::ShaderType::VERTEX_SHADER;
    }else if( ".gs" == ext ){
        type = gl::ShaderType::GEOMETRY_SHADER;
    }else if( ".tcs" == ext ){
        assert( false );
        type = gl::ShaderType::TESS_CONTROL_SHADER;
    }else if( ".tes" == ext ){
        assert( false );
        type = gl::ShaderType::TESS_EVALUATION_SHADER;
    }else if( ".comps" == ext ){
        assert( false );
        type = gl::ShaderType::COMPUTE_SHADER;
    }else{
        type = gl::ShaderType::NO_SHADER_TYPE;
    }
    // @TODO: add other shader extensions

    if( !silent ){
        if( gl::ShaderType::NO_SHADER_TYPE == type ){
            PT_LOG_INFO( "Failed to guess shader type from name '" << name << "'" );
        }else{
            PT_LOG_INFO( "Guessing shader type '" << gl::GetShaderTypeAsString(type) << "'from name '" << name << "'" );
        }
    }

    return type;
}
