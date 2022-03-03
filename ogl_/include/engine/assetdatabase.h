#pragma once

#include "assimp/scene.h"

#include <vector>

namespace pt{
namespace engine{

struct Material;
struct Mesh;
struct Texture;

struct MaterialDescriptor;
struct MeshDescriptor;
struct TextureDescriptor;

class AssetDatabase{
public:
    AssetDatabase(){}
    virtual ~AssetDatabase(){}
    AssetDatabase(const AssetDatabase& other)                   = delete;
    AssetDatabase(AssetDatabase&& source)                       = delete;
    AssetDatabase& operator=(const AssetDatabase& other) const  = delete;
    AssetDatabase& operator=(AssetDatabase&& source) const      = delete;
    bool operator==(const AssetDatabase& other) const           = delete;

    virtual void addMaterial(const MaterialDescriptor& descriptor) = 0;
    virtual void addMesh(const MeshDescriptor& descriptor) = 0;
    virtual void addTexture(const TextureDescriptor& descriptor) = 0;
    //virtual void addSound(const SoundDescriptor& descriptor) = 0;

    virtual void loadQueuedAssets() = 0;

    virtual const MaterialDescriptor&   getDescriptor(const Material& tex) const;
    virtual const MeshDescriptor&       getDescriptor(const Mesh& tex) const;
    virtual const TextureDescriptor&    getDescriptor(const Texture& tex) const;

};

} //end of namespace engine
} //end of namespace PT

