#pragma once

#include "engine/gl/Buffer.hpp"
#include "engine/gl/Material.h"

#include "pt/macros.h"
#include "pt/math.h"
#include <memory>
#include <vector>

namespace engine{

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
    Mesh();
    Mesh( const std::string& name );
    virtual ~Mesh(){}
    Mesh( Mesh&& source );
    Mesh& operator=( Mesh&& source );

    Mesh( const Mesh& other ) = delete;
    Mesh& operator=( const Mesh& other ) = delete;
    bool operator==( const Mesh& other ) const = delete;

    void            Clear();
    void            FreeClientsideData();
    void            FreeVRAM();
    const gl::Buffer<int>& GetIndexBuffer() const;
    const std::vector<gl::MaterialPtr>& GetMaterials() const;
    pt::Name        GetName() const;
    std::string     GetPath() const;
    const gl::Buffer<gl::Vertex>& GetVertexBuffer() const;

    const std::vector<int>& GetPieceIndexCounts() const;
    inline size_t   GetVRAMBytes() const;
    bool            IsLoadedInRAM() const;
    bool            IsLoadedInVRAM() const;
    void            LoadToGPU();
    void            Print();
    void            ReadFile( const std::string& path );

protected:
    using AdapterMap = std::unordered_map<std::string, std::string>;
    AdapterMap      ReadAdapterMap( const std::string& path );
    std::string     TranslateMaterialName( const std::string& name, AdapterMap& adapter ) const;
    std::string     FixMeshName( const std::string& name ) const;

private:
    std::string                     mName;
    std::vector<Piece>              mPieces;
    std::vector<int>                mPieceOffets;

    gl::Buffer<gl::Vertex>          mVertexBuffer;
    gl::Buffer<int>                 mIndexBuffer;
    std::vector<int>                mPieceIndexCount;

    std::vector<gl::MaterialPtr>    mCacheMaterials;
};

} // end of namespace 'engine'
