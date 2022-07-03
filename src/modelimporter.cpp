#include "engine/modelimporter.h"

#include "newmesh.h"
#include <assert.h>
#include <iostream>

using namespace pt;

/*
const aiScene *ModelImporter::getAiScene() const
{
    return mScene;
}
*/

bool pt::ModelImporter::
ReadModel(const char *path){

    const aiScene* scene;
    unsigned int params;

    //set properties to remove from mesh

    mImporter.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, 0
//                                      | aiComponent_NORMALS
//                                      | aiComponent_TANGENTS_AND_BITANGENTS
                                      | aiComponent_COLORS
//                                      | aiComponent_TEXCOORDS
                                      | aiComponent_BONEWEIGHTS
//                                      | aiComponent_ANIMATIONS
//                                      | aiComponent_TEXTURES
                                      | aiComponent_LIGHTS
                                      | aiComponent_CAMERAS
//                                      | aiComponent_MESHES
//                                      | aiComponent_MATERIALS
                                  );


    //http://sir-kimmi.de/assimp/lib_html/postprocess_8h.html

    params = 0
//            | aiProcess_CalcTangentSpace          //for normal mapping
            | aiProcess_JoinIdenticalVertices       //for indexed draws
//            | aiProcess_MakeLeftHanded            //for DirectX
            | aiProcess_Triangulate
//            | aiProcess_RemoveComponent             //remove parts defined by AI_CONFIG_PP_RVC_FLAGS  (seems to remove them anyway)
//            | aiProcess_GenNormals
//            | aiProcess_GenSmoothNormals
//            | aiProcess_SplitLargeMeshes
//            | aiProcess_PreTransformVertices
//            | aiProcess_LimitBoneWeights
            | aiProcess_ValidateDataStructure       //TODO: capture log after import
                                                        //http://sir-kimmi.de/assimp/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410ae420ce22fbbac9d0fd21fd92f2b630fa
//            | aiProcess_ImproveCacheLocality
//            | aiProcess_RemoveRedundantMaterials
//            | aiProcess_FixInfacingNormals
            | aiProcess_SortByPType
//            | aiProcess_FindDegenerates
//            | aiProcess_FindInvalidData
//            | aiProcess_GenUVCoords
            | aiProcess_TransformUVCoords
//            | aiProcess_FindInstances
//            | aiProcess_OptimizeMeshes
//            | aiProcess_OptimizeGraph
//            | aiProcess_FlipUVs                   //for DirectX
//            | aiProcess_FlipWindingOrder
//            | aiProcess_SplitByBoneCount
//            | aiProcess_Debone
    ;

    params = 0
            | aiProcess_JoinIdenticalVertices       //for indexed draws
            | aiProcess_Triangulate
            | aiProcess_RemoveComponent             //remove parts defined by AI_CONFIG_PP_RVC_FLAGS
            | aiProcess_ValidateDataStructure
            | aiProcess_SortByPType
            | aiProcess_TransformUVCoords

            //-----
            //the following are needed for Doom3 models
            | aiProcess_FlipUVs                   //for DirectX
            | aiProcess_FlipWindingOrder
            //-----
    ;



    if(nullptr == path){
        return false;
    }

    scene = mImporter.ReadFile(path, params);
    if(nullptr == scene){
        std::cout << "failed to read model file\n";
        return false;
    }

    mScene = scene;

    //PrintScene(mScene);

    return true;
}

bool ModelImporter::
ReadModel(const char *path, std::vector<pt::engine::NewMesh *> &out_meshes){
    if( !ReadModel(path) ){
        return false;
    }

    size_t meshcount = mScene->mNumMeshes;
    out_meshes.reserve(meshcount);
    for(size_t i=0; i<meshcount; ++i){
        out_meshes.push_back( new pt::engine::NewMesh( mScene->mMeshes[i]) );
    }
    return true;
}

bool ModelImporter::
ReadModel(const std::string &path){
    if( 0 == path.length() ){
        return false;
    }
    return ReadModel(path.c_str());
}

void ModelImporter::
PrintMeshVertices(const aiMesh *mesh) const{
    size_t size = mesh->mNumVertices;
    for(size_t i=0; i<size; ++i){
        aiVector3D vec = mesh->mVertices[i];
        PrintAiVector3D(vec);
    }
}

void ModelImporter::
PrintMeshFaces(const aiMesh *mesh) const{
    size_t size = mesh->mNumFaces;
    for(size_t j=0; j<size; ++j){
        std::cout << "-\n";
        for(size_t i=0; i<3; ++i){
            size_t index = mesh->mFaces[j].mIndices[i];
            aiVector3D vec = mesh->mVertices[index];
            PrintAiVector3D(vec);
        }
    }
}

void ModelImporter::
PrintAiVector3D(const aiVector3D &vec) const{
    std::cout <<"("<< vec.x <<","<< vec.y <<","<< vec.z << ")\n";
}

std::string ToString(bool b){
    if(b){  return std::string("true");
    }else{  return std::string("false");
    }
}

#include <iostream>
#include <vector>

void ModelImporter::
PrintScene(const aiScene *scene, const char* indent) const{
    bool HasLights      = scene->HasLights();
    bool HasMeshes      = scene->HasMeshes();
    bool HasCameras     = scene->HasCameras();
    bool HasTextures    = scene->HasTextures();
    bool HasMaterials   = scene->HasMaterials();
    bool HasAnimations  = scene->HasAnimations();

    size_t LightCount   = scene->mNumLights;
    size_t MeshCount    = scene->mNumMeshes;
    size_t CamCount     = scene->mNumCameras;
    size_t TexCount     = scene->mNumTextures;
    size_t MatCount     = scene->mNumMaterials;
    size_t AnimCount    = scene->mNumAnimations;


    std::cout << indent << "--------------------------------------------------\n";
    std::cout << indent << "scene:\n";
    std::cout << indent << "  HasLights:       "    << ToString(HasLights)     << "\n";
    std::cout << indent << "  HasMeshes:       "    << ToString(HasMeshes)     << "\n";
    std::cout << indent << "  HasCameras:      "    << ToString(HasCameras)    << "\n";
    std::cout << indent << "  HasTextures:     "    << ToString(HasTextures)   << "\n";
    std::cout << indent << "  HasMaterials:    "    << ToString(HasMaterials)  << "\n";
    std::cout << indent << "  HasAnimations:   "    << ToString(HasAnimations) << "\n";
    std::cout << indent << "  -----\n";
    std::cout << indent << "  LightCount: " <<  LightCount << "\n";
    std::cout << indent << "  MeshCount:  " <<  MeshCount << "\n";
    std::cout << indent << "  CamCount:   " <<  CamCount << "\n";
    std::cout << indent << "  TexCount:   " <<  TexCount << "\n";
    std::cout << indent << "  MatCount:   " <<  MatCount << "\n";
    std::cout << indent << "  AnimCount:  " <<  AnimCount << "\n";
    std::cout << indent << "  -----\n";

    std::vector<aiMesh*> meshes;
    meshes.resize(MeshCount);
    for(size_t i=0; i<MeshCount; ++i){
        meshes[i] = scene->mMeshes[i];
        aiMesh* current = meshes[i];
        PrintMesh(current, i, "  ");
        std::cout << indent << "  -----\n";
    }

    //process textures
    for(size_t i=0; i<scene->mNumTextures; ++i){
        aiTexture* tex = scene->mTextures[i];

        aiString name("");
        //aiString name = tex->mFilename;

        std::cout << indent << "-----\n";
        std::cout << indent << "  texture["<<i<<"]:\n";
        std::cout << indent << "    name: " << name.C_Str() << "\n";
    }

    //  scene->HasCameras
    //  scene->  HasTextures
    //  scene->HasMaterials
    //  scene->  HasAnimations


    std::cout << indent << "--------------------------------------------------\n";

}

void ModelImporter::
PrintMesh(const aiMesh *mesh, int idx, const char* indent) const{
    bool HasBones           = mesh->HasBones();
    bool HasFaces           = mesh->HasFaces();
    bool HasNormals         = mesh->HasNormals();
    bool HasPositions       = mesh->HasPositions();

    bool HasVertexColors    = mesh->HasVertexColors(0);
    bool HasTextureCoords   = mesh->HasTextureCoords(0);

    bool HasTangentsAndBitangents = mesh->HasTangentsAndBitangents();


    size_t  BoneCount         = mesh->mNumBones;
    size_t  FaceCount         = mesh->mNumFaces;
    size_t  VertexCount       = mesh->mNumVertices;
    size_t  AnimMeshCount     = mesh->mNumAnimMeshes;
    auto    UVComponentCount  = mesh->mNumUVComponents;
    size_t  UVChannelCount    = mesh->GetNumUVChannels();
    size_t  ColorChannelCount = mesh->GetNumColorChannels();

    size_t  MaxUVComponent = 0;
    while(MaxUVComponent < 7
          && UVComponentCount[MaxUVComponent] > 0){
        ++MaxUVComponent;
    }

    std::cout << indent << "  mesh[" << idx << "]:\n";
    std::cout << indent << "    name: "  << mesh->mName.C_Str() << "\n";
    std::cout << indent << "    HasBones:                 " << ToString(HasBones) << "\n";
    std::cout << indent << "    HasFaces:                 " << ToString(HasFaces) << "\n";
    std::cout << indent << "    HasNormals:               " << ToString(HasNormals) << "\n";
    std::cout << indent << "    HasPositions:             " << ToString(HasPositions) << "\n";
    std::cout << indent << "    HasVertexColors(0):       " << ToString(HasVertexColors) << "\n";
    std::cout << indent << "    HasTextureCoords(0):      " << ToString(HasTextureCoords) << "\n";
    std::cout << indent << "    HasTangentsAndBitangents: " << ToString(HasTangentsAndBitangents) << "\n";
    std::cout << indent << "    -----\n";
    std::cout << indent << "    BoneCount:           " << BoneCount << "\n";
    std::cout << indent << "    FaceCount:           " << FaceCount << "\n";
    std::cout << indent << "    VertexCount:         " << VertexCount << "\n";
    std::cout << indent << "    AnimMeshCount:       " << AnimMeshCount << "\n";
    std::cout << indent << "    UVChannelCount:      " << UVChannelCount << "\n";

    std::cout << indent << "    ColorChannelCount:   " << ColorChannelCount << "\n";

    //process UVComponents
    for(size_t i=0; i<MaxUVComponent; ++i){
        std::cout << indent << "    UVComponentCount["<<i<<"]: " << UVComponentCount[i] << "\n";
    }

    //process faces
    for(size_t i=0; i<mesh->mNumFaces; ++i){
        aiFace face = mesh->mFaces[i];
    }

    //process bones
    for(size_t i=0; i<mesh->mNumBones; ++i){
        aiBone* bone = mesh->mBones[i];
        std::cout << "-----\n";
        PrintBone(bone, i, "    ");
    }

/*
    for(int i=0; i<VertexCount; ++i){
        aiVector3D tex = mesh->mTextureCoords[0][i];
        size_t vectorsize = mesh->mNumUVComponents[0];
        assert(2 == vectorsize);
        std::cout << "texpos: " << tex.x << ", " << tex.y << "\n";
    }
*/



    for(size_t i=0; i<mesh->mNumBones; ++i){
    }


/*
    mesh->mBones
    mesh->mFaces
    mesh->mColors
    mesh->mMethod
    mesh->mNormals
    mesh->mNumBones
    mesh->mNumFaces
    mesh->mTangents

                mesh->mVertices

    mesh->mAnimMeshes
    mesh->mBitangents
    mesh->mMaterialIndex
    mesh->mTextureCoords
    mesh->mPrimitiveTypes
*/

}

void ModelImporter::
PrintBone(const aiBone *bone, int idx, const char *indent) const{
    std::cout << indent << "bone:\n";
    std::cout << indent << "  name["<<idx<<"]: " << bone->mName.C_Str() << "\n";
}

