/** -----------------------------------------------------------------------------
  * FILE:    Mesh.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Represents a mesh in 3d space, and its connections to related data (eg.: bones, materials).
  *          Handles transfer of data between File, RAM and VRAM.
  *          Does not handle drawing.
  * -----------------------------------------------------------------------------
  */

// TODO: move Assimp-based functionality behind a macro
// TODO: add glB and glTF support

#pragma once

#include "engine/gl/Buffer.hpp"
#include "engine/gl/Material.h"

#include "pt/macros.h"
#include "pt/math.h"
#include <memory>
#include <vector>

struct aiScene;

namespace engine{
namespace gl{

//Mesh::Piece - mesh data with a fixed material
//Mesh        - group of Pieces (a Mesh can have multiple materials)
PT_FORWARD_DECLARE_CLASS( Mesh )
class Mesh
{
    PT_FORWARD_DECLARE_STRUCT( Piece )
    struct Piece{
        std::string                 mName;
        std::vector< gl::Vertex >   mVertices;
        std::vector< math::int2 >   mEdges; // might be unnecessary
        std::vector< math::int3 >   mFaces;
        gl::MaterialPtr             mMaterial;

        Piece();
        Piece( const std::string& name );
        virtual ~Piece(){}
        Piece( Piece&& source );
        Piece& operator=( Piece&& source );

        void Clear();
    };

public:
    using AdapterMap    = std::unordered_map<std::string, std::string>;

    virtual ~Mesh(){}
    Mesh( Mesh&& source );
    Mesh& operator=( Mesh&& source );

    Mesh( const Mesh& other ) = delete;
    Mesh& operator=( const Mesh& other ) = delete;
    bool operator==( const Mesh& other ) const = delete;

    static MeshPtr  CreateFromAssimpScene( const std::string& name, const aiScene* scene, const AdapterMap* adapter = nullptr );
    static MeshPtr  CreateFromFileAssimp( const std::string& name ); // 'name' defines path

    void    FreeClientsideData();
    void    FreeVRAM();
    const gl::Buffer<int>&              GetIndexBuffer() const;
    const std::vector<gl::MaterialPtr>& GetMaterials() const;
    pt::Name                            GetName() const;
    std::string                         GetPath() const;
    const std::vector<size_t>&          GetPieceIndexCounts() const;
    const gl::Buffer<gl::Vertex>&       GetVertexBuffer() const;

    size_t  GetVRAMBytes() const;
    bool    IsLoadedInRAM() const;
    bool    IsLoadedInVRAM() const;
    void    LoadToGPU();
    void    Print();

protected:
    Mesh();
    Mesh( const std::string& name );

    static AdapterMap   CreateAdapterMap( const aiScene* scene );
    static std::string  FixMeshName( const std::string& name );
    static AdapterMap   ReadAdapterMap( const std::string& path );
    static std::string  TranslateMaterialName( const std::string& name, const AdapterMap& adapter );

private:
    inline void SetDefaultMemberValues(){
        mIsLoadedInVRAM     = false;
        mName               = std::string();
        mPieces             = std::vector<Piece>();
        mVertexBuffer       = gl::Buffer<gl::Vertex>();
        mIndexBuffer        = gl::Buffer<int>();
        mPieceIndexCount    = std::vector<size_t>();
        mMaterials          = std::vector<gl::MaterialPtr>();
    }

    bool                            mIsLoadedInVRAM = false;

    std::string                     mName;
    std::vector<Piece>              mPieces;

    gl::Buffer<gl::Vertex>          mVertexBuffer;
    gl::Buffer<int>                 mIndexBuffer;
    std::vector<size_t>             mPieceIndexCount;
    std::vector<gl::MaterialPtr>    mMaterials;
};

} // end of namespace 'gl'
} // end of namespace 'engine'
