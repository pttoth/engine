/** -----------------------------------------------------------------------------
  * FILE:    MeshLoader.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Handles loading meshes with Assimp and adds extra quality of life functionality.
  *          DEPRECATED !
  *          Class will be scrapped in the future...
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/experimental/Mesh.h"

#include "engine/Def.h"
#include "pt/macros.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <unordered_map>
#include <vector>

namespace engine{

PT_FORWARD_DECLARE_CLASS( MeshLoader )

//TODO: rename to AssetLoader
//TODO: memory safety!
class MeshLoader
{
public:
    MeshLoader();
    MeshLoader( const std::string& name );
    virtual ~MeshLoader();

    MeshLoader( const MeshLoader& other ) = delete;
    MeshLoader( MeshLoader&& source ) = delete;
    MeshLoader& operator=( const MeshLoader& other ) = delete;
    MeshLoader& operator=( MeshLoader&& source ) = delete;

    bool operator==( const MeshLoader& other ) const = delete;

    void AssimpSetPropertyInteger( const char *szName, int iValue );
    const aiScene* AssimpLoadMesh( const std::string& path, unsigned int pFlags );
    void FreeScene();

    //TODO: move these out to some ToString() function (either global, or class member)
    void PrintScene( const aiScene* scene, const char* indent ) const;
    void PrintMesh( const aiMesh* mesh, int idx = 0, const char* indent = "" ) const;
    void PrintBone( const aiBone* bone, int idx = 0, const char* indent = "" ) const;


protected:
    bool            ReadAdapterMap( const std::string& path );


private:
    Assimp::Importer    mImporter;
};

} // end of namespace 'engine'
