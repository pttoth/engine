/** -----------------------------------------------------------------------------
  * FILE:    AssetControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Manages parsing, locating, loading, storage and lifecycle of engine resources.
  * -----------------------------------------------------------------------------
  */

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

    virtual void                    SafeReleaseMesh( const std::string& name ) = 0;

    virtual gl::MaterialPtr         GetFallbackMaterial() = 0;
    virtual gl::MeshPtr             GetFallbackMesh() = 0;
    virtual gl::Texture2dPtr        GetFallbackTexture() = 0;
    virtual gl::ShaderProgramPtr    GetFallbackShaderProgram() = 0;
    virtual gl::MaterialPtr         GetMaterial( const std::string& name ) = 0;
    virtual gl::MeshPtr             GetMesh( const std::string& name ) = 0;
    virtual MeshLoaderPtr           GetMeshLoader() = 0; // DEPRECATED
    virtual gl::Texture2dPtr        GetTexture( const std::string& name ) = 0;
    virtual gl::ShaderPtr           GetShader( const std::string& name ) = 0;
    virtual gl::ShaderProgramPtr    GetShaderProgram( const std::string& name ) = 0;

    virtual bool                    LoadMaterial( const std::string& name ) = 0;
    virtual bool                    LoadMesh( const std::string& name, gl::Mesh::FormatHint hint = gl::Mesh::FormatHint::MD5_IDTECH4 ) = 0;
    virtual bool                    LoadShader( const std::string& name, gl::ShaderType type = gl::ShaderType::NO_SHADER_TYPE ) = 0;
    virtual bool                    LoadTexture( const std::string& name ) = 0;

    virtual std::string             ResolveAssimpConfigFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMaterialFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshAdapterFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshFileName( const std::string& name, gl::Mesh::FormatHint hint = gl::Mesh::FormatHint::MD5_IDTECH4 ) = 0;
    virtual std::string             ResolveShaderFileName( const std::string& name ) = 0;
    virtual std::string             ResolveTextureFileName( const std::string& name ) = 0;

    virtual bool                    SetFallbackMaterial( gl::MaterialPtr material ) = 0;
    virtual bool                    SetFallbackMaterialTexture( gl::Texture2dPtr texture ) = 0;
    virtual bool                    SetFallbackShaderProgram( gl::ShaderProgramPtr shaderprogram ) = 0;
    virtual bool                    SetFallbackTexture( gl::Texture2dPtr texture ) = 0;

    virtual bool AddMaterial( gl::MaterialPtr material ) = 0;
    virtual void RemoveMaterial( const std::string& name ) = 0;
    virtual bool AddMesh( gl::MeshPtr mesh ) = 0;
    virtual void RemoveMesh( const std::string& name ) = 0;
    virtual bool AddShader( gl::ShaderPtr shader ) = 0;
    virtual void RemoveShader( const std::string& name ) = 0;
    virtual bool AddShaderProgram( gl::ShaderProgramPtr shaderprogram ) = 0;
    virtual void RemoveShaderProgram( const std::string& name ) = 0;
    virtual bool AddTexture( gl::Texture2dPtr texture ) = 0;
    virtual void RemoveTexture( const std::string& name ) = 0;
protected:

private:

};

} // end of namespace 'engine'
