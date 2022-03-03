#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include "newmesh.h"

namespace pt{

class ModelImporter{
    Assimp::Importer    mImporter;
    const aiScene*      mScene;
public:
    ModelImporter(){}
    ~ModelImporter(){}

    const aiScene* getAiScene() const;

    bool ReadModel(const char* path);
    bool ReadModel(const char *path, std::vector<pt::engine::NewMesh*> &out_meshes);
    bool ReadModel(const std::string& path);

    void PrintScene(const aiScene* scene, const char* indent = "") const;
    void PrintMesh(const aiMesh* mesh, int idx = 0, const char* indent = "") const;
    void PrintBone(const aiBone* bone, int idx = 0, const char* indent = "") const;

    void PrintMeshVertices(const aiMesh* mesh) const;
    void PrintMeshFaces(const aiMesh* mesh) const;
    void PrintAiVector3D(const aiVector3D& vec)const;


};

} //end of namespace PT


