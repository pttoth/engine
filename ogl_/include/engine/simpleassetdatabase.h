#pragma once

#include "engine/assetdatabase.h"

#include "engine/material.h"
#include "engine/mesh.h"
#include "engine/texture.h"

namespace Assimp{
    class Importer;
}

namespace pt{
namespace engine{

class SimpleAssetDatabase: public AssetDatabase
{
public:
    SimpleAssetDatabase();
    virtual ~SimpleAssetDatabase() override;

    SimpleAssetDatabase(const SimpleAssetDatabase& other)                   = delete;
    SimpleAssetDatabase(SimpleAssetDatabase&& source)                       = delete;
    SimpleAssetDatabase& operator=(const SimpleAssetDatabase& other) const  = delete;
    SimpleAssetDatabase& operator=(SimpleAssetDatabase&& source) const      = delete;
    bool operator==(const SimpleAssetDatabase& other) const                 = delete;

    // AssetDatabase interface
    virtual void addMaterial(const MaterialDescriptor &descriptor) override;
    virtual void addMesh(const MeshDescriptor &descriptor) override;
    virtual void addTexture(const TextureDescriptor &descriptor) override;
    virtual void loadQueuedAssets() override;

    const MaterialDescriptor&   getDescriptor(const Material& tex) const override;
    const MeshDescriptor&       getDescriptor(const Mesh& tex) const override;
    const TextureDescriptor&    getDescriptor(const Texture& tex) const override;

protected:
    Assimp::Importer*               mAssImporter;
    std::vector<const aiScene*>     mAIScenes;

    std::vector<Mesh*>  mMeshes;
    std::vector<Texture*>           mTextures;
    std::vector<Material*>          mMaterials;


    bool isMaterialContained(const std::string& name) const;
    bool isMeshContained(const std::string& name) const;
    bool isTextureContained(const std::string& name) const;

    bool isContained(const Texture* tex) const;
    bool isContained(const Material* mat) const;
    bool isContained(const Mesh* mesh) const;

    Material*   loadMaterial(const MaterialDescriptor& descriptor);
    Mesh*       loadMesh(const MeshDescriptor& descriptor);

};

/*
class AssetDatabase{
    std::vector<const aiScene*>     mAIScenes;
    std::vector<pt::engine::Mesh*>  mMeshes;


    ...
};
*/

} //end of namespace engine
} //end of namespace PT




