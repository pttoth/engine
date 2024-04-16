#pragma once

#include "AssetControl.h"

#include "engine/Def.h"
#include "pt/macros.h"
#include <map>


namespace engine{

PT_FORWARD_DECLARE_CLASS( AssetManager )

class AssetManager: public AssetControl
{
public:
    AssetManager();
    virtual ~AssetManager();

    AssetManager( const AssetManager& other ) = delete;
    AssetManager( AssetManager&& source ) = delete;
    AssetManager& operator=( const AssetManager& other ) = delete;
    AssetManager& operator=( AssetManager&& source ) = delete;
    bool operator==( const AssetManager& other ) const = delete;

    //TODO: determine which one is better 'pt::Name' or 'std::string'
    //  one-time searches without preconstruction common?
    gl::MaterialPtr         GetMaterial( const std::string& path ) override;
    MeshPtr                 GetMesh( const std::string& name ) override;
    MeshLoaderPtr           GetMeshLoader() override;                       //TODO: rename to AssimpMeshLoader
    gl::Texture2dPtr        GetTexture( const std::string& name ) override;
    gl::ShaderPtr           GetShader( const pt::Name& name ) override;
    gl::ShaderProgramPtr    GetShaderProgram( const pt::Name& name ) override;

    bool                    LoadTexture( const std::string& name ) override;

    //bool                    ReadMeshFileMD5( const std::string& path ) override;
    std::string             ResolveAssimpConfigFileName( const std::string& name ) override;
    std::string             ResolveMaterialFileName( const std::string& name ) override;
    std::string             ResolveMeshAdapterFileName( const std::string& name ) override;
    std::string             ResolveMeshFileName( const std::string& name ) override;
    std::string             ResolveTextureFileName( const std::string& name ) override;

    void AddShader( gl::ShaderPtr shader ) override;
    void RemoveShader( const pt::Name& name ) override;
    void AddShaderProgram( gl::ShaderProgramPtr shaderprogram ) override;
    void RemoveShaderProgram( const pt::Name& name ) override;

protected:

private:
    //TODO: refactor to use pt::Name
    std::unordered_map<std::string, gl::MaterialPtr>    mMaterials;
    std::unordered_map<std::string, MeshPtr>            mMeshes;
    std::unordered_map<pt::Name, gl::ShaderPtr>         mShaders;
    std::unordered_map<pt::Name, gl::ShaderProgramPtr>  mShaderPrograms;
    std::unordered_map<std::string, gl::Texture2dPtr>   mTextures;

    MeshLoaderPtr mMeshLoader;

    bool mLateFetchEnabled = true; //TODO: disable
};

} // end of namespace 'engine'
