#pragma once

#include "engine/gl/Texture2d.h"
#include "engine/gl/Material.h"
#include "engine/gl/Mesh.h"
#include "engine/gl/Shader.h"
#include "engine/gl/ShaderProgram.h"

#include "engine/Def.h"
#include "engine/MeshLoader.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( AssetControl )

class AssetControl
{
public:
    virtual ~AssetControl(){}

    virtual gl::MaterialPtr         GetFallbackMaterial() = 0;
    virtual gl::MeshPtr             GetFallbackMesh() = 0;
    virtual gl::Texture2dPtr        GetFallbackTexture() = 0;
    virtual gl::MaterialPtr         GetMaterial( const std::string& path ) = 0;
    virtual gl::MeshPtr             GetMesh( const std::string& name ) = 0;
    virtual MeshLoaderPtr           GetMeshLoader() = 0;
    virtual gl::Texture2dPtr        GetTexture( const std::string& path ) = 0;
    virtual gl::ShaderPtr           GetShader( const pt::Name& name ) = 0;
    virtual gl::ShaderProgramPtr    GetShaderProgram( const pt::Name& name ) = 0;

    virtual bool                    LoadTexture( const std::string& name ) = 0;
    virtual bool                    LoadMaterial( const std::string& name ) = 0;
    virtual bool                    LoadMesh( const std::string& name ) = 0;

    virtual std::string             ResolveAssimpConfigFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMaterialFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshAdapterFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshFileName( const std::string& name ) = 0;
    virtual std::string             ResolveTextureFileName( const std::string& name ) = 0;

    virtual void AddShader( gl::ShaderPtr shader ) = 0;
    virtual void RemoveShader( const pt::Name& name ) = 0;
    virtual void AddShaderProgram( gl::ShaderProgramPtr shaderprogram ) = 0;
    virtual void RemoveShaderProgram( const pt::Name& name ) = 0;
protected:

private:

};

} // end of namespace 'engine'