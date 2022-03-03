#include "engine/simpleassetdatabase.h"

#include "assimp/Importer.hpp"

#include "pt/std_extension.hpp"

#include <cstring>

using namespace pt;
using namespace pt::engine;



SimpleAssetDatabase::
SimpleAssetDatabase(){
    mAssImporter = new Assimp::Importer();
}

SimpleAssetDatabase::
~SimpleAssetDatabase(){
    delete mAssImporter;
}

void SimpleAssetDatabase::
addMaterial(const MaterialDescriptor& descriptor){
    Material* mat;
    if( !isMaterialContained(descriptor.name) ){
        mat = loadMaterial(descriptor);
        if(nullptr != mat){
            mMaterials.push_back(mat);
        }else{
            std::cout << "Asset Load Error: could not load asset: " << descriptor.name << "\n";
        }
    }
}

void SimpleAssetDatabase::
addMesh(const MeshDescriptor &descriptor){
    Mesh* mesh;
    if( !isMeshContained(descriptor.name) ){
        mesh = loadMesh(descriptor);
        if(nullptr != mesh){
            mMeshes.push_back(mesh);
        }else{
            std::cout << "Asset Load Error: could not load asset: " << descriptor.name << "\n";
        }
    }
}

void SimpleAssetDatabase::
addTexture(const TextureDescriptor &descriptor){
    //TODO: move texture loading from Texture to this class!
    Texture* tex = new Texture(descriptor);
    if( !isContained(tex) ){
        tex->loadToGPUFromFile(descriptor.name);
        //TODO: how to check 4 errors here?
        mTextures.push_back(tex);
    }else{
        delete tex;
    }
}

void SimpleAssetDatabase::
loadQueuedAssets(){
    //this implementation doesn't actually do anything here
}

const MaterialDescriptor &SimpleAssetDatabase::
getDescriptor(const Material &tex) const{

}

const MeshDescriptor &SimpleAssetDatabase::
getDescriptor(const Mesh &tex) const{

}

const TextureDescriptor &SimpleAssetDatabase::
getDescriptor(const Texture &tex) const{

}

bool SimpleAssetDatabase::
isTextureContained(const std::string& tex_name) const{
    for( size_t i=0; i<mTextures.size(); ++i ){
        if( mTextures[i]->getName() == tex_name ){
            return true;
        }
    }
    return false;
}

bool SimpleAssetDatabase::
isMaterialContained(const std::string &mat_name) const{
    for( size_t i=0; i<mMaterials.size(); ++i ){
        if( mMaterials[i]->getName() == mat_name ){
            return true;
        }
    }
    return false;
}

bool SimpleAssetDatabase::
isMeshContained(const std::string &mesh_name) const{
    for( size_t i=0; i<mMeshes.size(); ++i ){
        if( mMeshes[i]->getName() == mesh_name ){
            return true;
        }
    }
    return false;
}

bool SimpleAssetDatabase::
isContained(const Texture *tex) const{
    return isTextureContained(tex->getName());
}

bool SimpleAssetDatabase::
isContained(const Material *mat) const{
    return isTextureContained(mat->getName());
}

bool SimpleAssetDatabase::
isContained(const Mesh *mesh) const{
    return isTextureContained(mesh->getName());
}

Material *SimpleAssetDatabase::loadMaterial(const MaterialDescriptor &descriptor)
{

}

Mesh *SimpleAssetDatabase::loadMesh(const MeshDescriptor &descriptor)
{

}
