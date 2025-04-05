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

    void                    SafeReleaseMesh( const std::string& name ) override;

    gl::MaterialPtr         GetFallbackMaterial() override;
    gl::MeshPtr             GetFallbackMesh() override;
    gl::Texture2dPtr        GetFallbackTexture() override;
    gl::ShaderPtr           GetFallbackShader( gl::ShaderType type ) override;
    gl::ShaderProgramPtr    GetFallbackShaderProgram() override;
    gl::MaterialPtr         GetMaterial( const std::string& name ) override;
    gl::MeshPtr             GetMesh( const std::string& name ) override;
    MeshLoaderPtr           GetMeshLoader() override; // DEPRECATED
    gl::Texture2dPtr        GetTexture( const std::string& name ) override;

    // returns 'nullptr', if shader type cannot be deduced from 'name'
    gl::ShaderPtr           GetShader( const std::string& name ) override;

    // This ONLY returns 'nullptr', if 'type' is 'NO_SHADER_TYPE'
    //  or if fallback shaders are not set
    gl::ShaderPtr           GetShader( const std::string& name, gl::ShaderType type ) override;

    gl::ShaderProgramPtr    GetShaderProgram( const std::string& name ) override;

    // Load<asset type>():
    //   Resolves path based on 'name'
    //   Loads specified asset
    //   Ensures, that there will be an entry with 'name' (either the correct instance, or a stub)
    // Only returns 'true', if after the call, an existing, correctly loaded and non-stub asset is stored with 'name'.
    //   The object's data at this point may still be invalid, but correctly-formatted.
    //   Such objects are mandated not to break execution, they turn into stubs, when invalid data is encountered during usage.
    bool                    LoadMaterial( const std::string& name, bool force = false ) override;
    bool                    LoadMesh( const std::string& name, gl::Mesh::FormatHint hint = gl::Mesh::FormatHint::MD5_IDTECH4, bool force = false ) override;
    bool                    LoadShader( const std::string& name ) override;
    bool                    LoadShader( const std::string& name, gl::ShaderType type, bool force = false ) override;
    bool                    LoadShaderProgram( const std::string& name, bool force = false ) override;
    bool                    LoadTexture( const std::string& name, bool force = false ) override;

    std::string             ResolveAssimpConfigFileName( const std::string& name ) override;
    std::string             ResolveMaterialFileName( const std::string& name ) override;
    std::string             ResolveMeshAdapterFileName( const std::string& name ) override;
    std::string             ResolveMeshFileName( const std::string& name, gl::Mesh::FormatHint hint = gl::Mesh::FormatHint::MD5_IDTECH4 ) override;
    std::string             ResolveShaderFileName( const std::string& name ) override;
    std::string             ResolveTextureFileName( const std::string& name ) override;

    void                    SetFallbackMaterial( gl::MaterialPtr material ) override;
    void                    SetFallbackMaterialTexture( gl::Texture2dPtr texture ) override;
    void                    SetFallbackShader( gl::ShaderPtr shader, gl::ShaderType type ) override;
    void                    SetFallbackShaderProgram( gl::ShaderProgramPtr shaderprogram ) override;
    void                    SetFallbackTexture( gl::Texture2dPtr texture ) override;


    bool                    AddMaterial( gl::MaterialPtr material ) override;
    void                    RemoveMaterial( const std::string& name ) override;
    bool                    AddMesh( gl::MeshPtr mesh ) override;
    void                    RemoveMesh( const std::string& name ) override;
    bool                    AddShader( gl::ShaderPtr shader ) override;
    void                    RemoveShader( const std::string& name ) override;
    bool                    AddShaderProgram( gl::ShaderProgramPtr shaderprogram ) override;
    void                    RemoveShaderProgram( const std::string& name ) override;
    bool                    AddTexture( gl::Texture2dPtr texture ) override;
    void                    RemoveTexture( const std::string& name ) override;

protected:
    static gl::ShaderType   GuessShaderTypeByName( const std::string& name );

    // Find material without returning fallback if missing
    gl::MaterialPtr         FindMaterial( const std::string& name ) const;

private:
    gl::MaterialPtr                                 mFallbackMaterial;
    gl::Texture2dPtr                                mFallbackMaterialTexture;
    std::unordered_map<gl::ShaderType, gl::ShaderPtr> mFallbackShaders;
    gl::ShaderProgramPtr                            mFallbackShaderProgram;
    gl::Texture2dPtr                                mFallbackTexture;

    std::unordered_map<std::string, gl::MaterialPtr>    mMaterials;
    std::unordered_map<std::string, gl::MeshPtr>        mMeshes;
    std::unordered_map<std::string, gl::ShaderPtr>        mShaders;
    std::unordered_map<std::string, gl::ShaderProgramPtr> mShaderPrograms;
    std::unordered_map<std::string, gl::Texture2dPtr>   mTextures;

    MeshLoaderPtr mMeshLoader;
};

} // end of namespace 'engine'
