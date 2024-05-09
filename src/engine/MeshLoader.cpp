#include "engine/MeshLoader.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/gl/AssimpConfig.h"
#include "engine/gl/Mesh.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


const std::string&
ToString( bool b ){
    static const std::string str_true( "true" );
    static const std::string str_false( "false" );
    return b ? str_true : str_false;
}


using namespace engine;

MeshLoader::
MeshLoader()
{}


void MeshLoader::
AssimpSetPropertyInteger( const char* szName, int iValue )
{
    mImporter.SetPropertyInteger( szName, iValue );
}


const aiScene* MeshLoader::
AssimpLoadMesh( const std::string& path, unsigned int pFlags )
{
    return mImporter.ReadFile( path, pFlags );
}


void MeshLoader::
FreeScene()
{
    mImporter.FreeScene();
}


MeshLoader::
~MeshLoader()
{}


/*
bool MeshLoader::
ReadMesh( const std::string& path , const std::string& name ){
    if( 0 == name.length() ){
        PT_LOG_ERR( "MeshLoader::ReadModel(): empty name as parameter" );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace();
        #endif
    }

    PT_LOG_INFO( "Loading model: '" << path << "'" );
    std::string prefix = path + "/" + name;

    std::string mesh_path = prefix + ".md5mesh";
    std::string cfg_path = prefix + ".assimp.cfg";
    std::string adapter_path = prefix + ".adapter.txt";

    AssimpConfig cfg;
    try{
        cfg.readF( cfg_path );
    }catch( const std::invalid_argument& e ){
        return false;
    }

    //TODO: could make a global adaptermap, that'll translate all Doom models?
    if( !ReadAdapterMap( adapter_path ) ){
        PT_LOG_DEBUG( "Couldn't read or non-existent adapter info for '" << prefix << "'" );
    }

    //aiComponent
    int componentsToRemove = 0;
    //componentsToRemove |= aiComponent_NORMALS;
    //componentsToRemove |= aiComponent_TANGENTS_AND_BITANGENTS;
    //componentsToRemove |= aiComponent_COLORS;
    //componentsToRemove |= aiComponent_TEXCOORDS;
    //componentsToRemove |= aiComponent_BONEWEIGHTS;
    //componentsToRemove |= aiComponent_ANIMATIONS;
    //componentsToRemove |= aiComponent_TEXTURES;
    //componentsToRemove |= aiComponent_LIGHTS;
    //componentsToRemove |= aiComponent_CAMERAS;
    //componentsToRemove |= aiComponent_MESHES;
    //componentsToRemove |= aiComponent_MATERIALS;
    mImporter.SetPropertyInteger( AI_CONFIG_PP_RVC_FLAGS, componentsToRemove );

    const aiScene* scene = mImporter.ReadFile( mesh_path, cfg.GetParameterMask() );
    if( nullptr == scene ){
        PT_LOG_ERR( "Failed to load mesh: '" << mesh_path << "'" );
        return false;
    }
    // mImporter safely handles memory, but this prevents it unnecessarily holding copies of mesh data
    auto guard = pt::CreateGuard( [this]{
        mImporter.FreeScene();
    } );


    auto ac = Services::GetAssetControl();

    Mesh mesh( scene->mName.C_Str() );
    mesh.mComponents.reserve( scene->mNumMeshes );
    for( size_t idx_component=0; idx_component<scene->mNumMeshes; ++idx_component ){
        aiMesh* component = scene->mMeshes[idx_component];
        Mesh::Component meshComponent( FixMeshName( component->mName.C_Str() ) );
        meshComponent.mVertices.reserve( component->mNumVertices );
        meshComponent.mFaces.reserve( component->mNumFaces );

        //setup material
        if( 0 == mAdapterMap.count( meshComponent.mName ) ){
            PT_LOG_WARN( "Could not find mesh name '" << meshComponent.mName << "' in adapter map '" << adapter_path << "'" );
        }else{
            PT_LOG_DEBUG( "Replaced name '" << meshComponent.mName << "' to '"<< mAdapterMap[meshComponent.mName] << "' by adapter map '" << adapter_path << "'" );
            meshComponent.mName = mAdapterMap[meshComponent.mName];
        }
        meshComponent.mMaterial = ac->GetMaterial( ResolveFileName(meshComponent.mName) );

        //setup vertices
        for( size_t idx_vertex=0; idx_vertex<component->mNumVertices; ++idx_vertex ){
            const aiVector3D& v = component->mVertices[idx_vertex];
            meshComponent.mVertices.push_back( math::float3( v.x, v.y, v.z ) );
        }

        //set up faces
        for( size_t idx_face=0; idx_face<component->mNumFaces; ++idx_face ){
            aiFace& face = component->mFaces[idx_face];
            if( 3 != face.mNumIndices ){
                PT_LOG_DEBUG( "Face vertex count differs from '3' while loading mesh '" << path << "'" );
                PT_UNIMPLEMENTED_FUNCTION
                //TODO: break up face into triangles
            }
            meshComponent.mFaces.push_back( math::int3( face.mIndices[0], face.mIndices[1], face.mIndices[2] ) );
        }

        mesh.mComponents.push_back( std::move( meshComponent ) );
    }

    //mMesh = std::move(mesh);
    return true;
}
*/

void MeshLoader::
PrintScene(const aiScene *scene, const char* indent) const
{
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
        //aiTexture* tex = scene->mTextures[i];

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

void MeshLoader::
PrintMesh(const aiMesh *mesh, int idx, const char* indent) const
{
    bool HasBones           = mesh->HasBones();
    bool HasFaces           = mesh->HasFaces();
    bool HasNormals         = mesh->HasNormals();
    bool HasPositions       = mesh->HasPositions();

    bool HasVertexColors    = mesh->HasVertexColors(0);
    bool HasTextureCoords0  = mesh->HasTextureCoords(0);
    bool HasTextureCoords1  = mesh->HasTextureCoords(1);

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
    std::cout << indent << "    HasTextureCoords(0):      " << ToString(HasTextureCoords0) << "\n";
    std::cout << indent << "    HasTextureCoords(1):      " << ToString(HasTextureCoords1) << "\n";
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
    std::cout << indent << "    bones:\n";
    for(size_t i=0; i<mesh->mNumBones; ++i){
        aiBone* bone = mesh->mBones[i];
        PrintBone(bone, i, "        ");
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


void MeshLoader::
PrintBone(const aiBone *bone, int idx, const char *indent) const
{
    std::cout << indent << "bone["<<idx<<"]: " << bone->mName.C_Str() << "\n";
}


