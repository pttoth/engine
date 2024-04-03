#pragma once

#include "engine/gl/Texture2d.h"
#include "engine/gl/Material.h"
#include "engine/experimental/Mesh.h"
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

    virtual bool PrefetchTexture( const std::string& path ) = 0;
    virtual gl::MaterialPtr         GetMaterial( const std::string& path ) = 0;
    virtual MeshPtr                 GetMesh( const std::string& name ) = 0;
    virtual MeshLoaderPtr           GetMeshLoader() = 0;
    virtual gl::Texture2dPtr        GetTexture( const std::string& path ) = 0;
    virtual gl::ShaderPtr           GetShader( const pt::Name& name ) = 0;
    virtual gl::ShaderProgramPtr    GetShaderProgram( const pt::Name& name ) = 0;

    virtual std::string             ResolveAssimpConfigFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMaterialFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshAdapterFileName( const std::string& name ) = 0;
    virtual std::string             ResolveMeshFileName( const std::string& name ) = 0;

    virtual void AddShader( gl::ShaderPtr shader ) = 0;
    virtual void RemoveShader( const pt::Name& name ) = 0;
    virtual void AddShaderProgram( gl::ShaderProgramPtr shaderprogram ) = 0;
    virtual void RemoveShaderProgram( const pt::Name& name ) = 0;
protected:

private:

};

} // end of namespace 'engine'
