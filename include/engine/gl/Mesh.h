/** -----------------------------------------------------------------------------
  * FILE:    Mesh.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Represents a mesh in 3d space, and its connections to related data (eg.: bones, materials).
  *          Handles transfer of data between File, RAM and VRAM.
  *          Does not handle drawing.
  * -----------------------------------------------------------------------------
  */

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

// TODO: move Assimp-based functionality behind a macro
// TODO: add glB and glTF support

// @TODO: add support for both smooth and per-triangle vertex normals

// @TODO: 'mNormalBuffer' is only needed in VRAM for drawing normal vectors in wireframe mode
//          implement some on-demand upload for it
//          take into account, that usage can become inconsistent, when trying to generate normal buffer after deleting the RAM-side of the vertex buffer

// @TODO: implement VAO usage



//Mesh::Piece - mesh data with one specific material
//Mesh        - group of Pieces | object representing a complex, multi-material object
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
    enum FormatHint{
        MD5_IDTECH4 = 0,
        GLTF        = 1,
        //FBX
    };

    using AdapterMap    = std::unordered_map<std::string, std::string>;

    virtual ~Mesh(){}

    Mesh( const Mesh& other ) = delete;
    Mesh& operator=( const Mesh& other ) = delete;
    bool operator==( const Mesh& other ) const = delete;

    static MeshPtr  CreateFromSceneAssimp( const std::string& name, const aiScene* scene, const AdapterMap* adapter = nullptr );
    static MeshPtr  CreateStubMesh( const std::string& name );

    // @TODO: refactor this !!!
    //          take a name, a path and a hint
    //          it is AssetControl's job to deduce path from name!
    static MeshPtr  CreateFromFile( const std::string& name, FormatHint hint ); // 'name' defines path

    static void     Initialize();

    void    FreeClientsideData();
    void    FreeVRAM();
    const gl::Buffer<int>&              GetIndexBuffer() const;
    const std::vector<gl::MaterialPtr>& GetMaterials() const;
    const std::string&                  GetName() const;
    const gl::Buffer<math::vec3>&       GetNormalBuffer() const;
    std::string                         GetPath() const;
                                                                        // @TODO: add GetPieceCount()
    const std::vector<size_t>&          GetPieceIndexCounts() const;    // @TODO: get piece index as param, return size
    const gl::Buffer<gl::Vertex>&       GetVertexBuffer() const;

    size_t  GetVRAMBytes() const;
    bool    IsLoadedInRAM() const;
    bool    IsLoadedInVRAM() const;
    bool    IsStub() const;
    void    LoadToGPU();
    void    Print();

protected:
    Mesh();
    Mesh( const std::string& name );

    static AdapterMap   CreateAdapterMap( const aiScene* scene );
    static std::string  FixMeshName( const std::string& name );
    static std::vector<math::vec3> GenerateNormalVectorCoordinates( const std::vector<gl::Vertex>& vertices );
    static AdapterMap   ReadAdapterMap( const std::string& path );
    static std::string  TranslateMaterialName( const std::string& name, const AdapterMap& adapter );

private:
    static MeshPtr                  stFallbackMesh;

    static const float              stNormalsLength;    // displayed normal vector length

    bool                            mIsLoadedInVRAM = false;

    std::string                     mName;
    std::vector<Piece>              mPieces;

    gl::Buffer<gl::Vertex>          mVertexBuffer;
    gl::Buffer<int>                 mIndexBuffer;
    gl::Buffer<math::vec3>          mNormalBuffer;      // used for displaying the normal vectors as lines (mVertexBuffer contains normal data for other usages)
    std::vector<size_t>             mPieceIndexCount;
    std::vector<gl::MaterialPtr>    mMaterials; //@TODO: rename to mCachedMaterials
                                                //@TODO: add dirty flag?
};

} // end of namespace 'gl'
} // end of namespace 'engine'
