#pragma once

#include "pt/math.h"
#include "engine/asset.h"
#include "engine/material.h"

#include <vector>

namespace pt{
namespace engine{

class IRenderer;

struct Mesh;
struct Light;

//interface
class IRenderer{
    virtual void loadToGPU(Mesh* mesh) = 0;
    virtual void loadToGPU(Texture* texture) = 0;
    virtual void loadToGPU(Light* light) = 0;

};

struct Vertex{
    math::float3 mPosition;
    math::float3 mNormal;
    math::float2 mTexturePosition;
};


struct MeshDescriptor: public AssetDescriptor{


}; // end of MeshDescriptor

//currently only support triangles
struct Face{
    std::vector<uint32_t>   iVertices;
};

struct Mesh: Asset{
    std::vector<Vertex>     mVertices;
    std::vector<Face>       mFaces;
    std::vector<Material*>  mMaterials;
    uint32_t                mVAI, mVBO, mIBO;

    Mesh();
    Mesh(const MeshDescriptor& descriptor);
    virtual ~Mesh() override;
    size_t  getVertexCount() const;
    size_t  getFaceCount() const;
    size_t  getMaterialCount() const;

    void    loadToGPU(IRenderer* renderer);
    void    releaseFromGPU(IRenderer* renderer);

    void    draw(IRenderer* renderer) const;

};


} //end of namespace engine










class Cube: public engine::Mesh{
    Cube(){
        //mName = "Cube";
        engine::Vertex v1,v2,v3,v4, v5,v6,v7,v8;
        mVertices.reserve(8);
        v1.mPosition = math::float3(0.0f, 0.0f, 0.0f);
        v2.mPosition = math::float3(0.0f, 1.0f, 0.0f);
        v3.mPosition = math::float3(1.0f, 0.0f, 0.0f);
        v4.mPosition = math::float3(1.0f, 1.0f, 0.0f);
        v5.mPosition = math::float3(0.0f, 0.0f, 1.0f);
        v6.mPosition = math::float3(0.0f, 1.0f, 1.0f);
        v7.mPosition = math::float3(1.0f, 0.0f, 1.0f);
        v8.mPosition = math::float3(1.0f, 1.0f, 1.0f);
        mVertices.push_back( v1 );
        mVertices.push_back( v2 );
        mVertices.push_back( v3 );
        mVertices.push_back( v4 );
        mVertices.push_back( v5 );
        mVertices.push_back( v6 );
        mVertices.push_back( v7 );
        mVertices.push_back( v8 );
        engine::Face faces[12];
        faces[0].iVertices.push_back(0);    faces[1].iVertices.push_back(2);    faces[2].iVertices.push_back(0);
        faces[0].iVertices.push_back(1);    faces[1].iVertices.push_back(3);    faces[2].iVertices.push_back(1);
        faces[0].iVertices.push_back(2);    faces[1].iVertices.push_back(0);    faces[2].iVertices.push_back(4);

        faces[3].iVertices.push_back(1);    faces[4].iVertices.push_back(1);    faces[5].iVertices.push_back(2);
        faces[3].iVertices.push_back(4);    faces[4].iVertices.push_back(2);    faces[5].iVertices.push_back(5);
        faces[3].iVertices.push_back(5);    faces[4].iVertices.push_back(5);    faces[5].iVertices.push_back(6);

        faces[6].iVertices.push_back(2);    faces[7].iVertices.push_back(3);    faces[8].iVertices.push_back(3);
        faces[6].iVertices.push_back(3);    faces[7].iVertices.push_back(6);    faces[8].iVertices.push_back(0);
        faces[6].iVertices.push_back(6);    faces[7].iVertices.push_back(7);    faces[8].iVertices.push_back(5);

        faces[9].iVertices.push_back(0);    faces[10].iVertices.push_back(5);   faces[11].iVertices.push_back(7);
        faces[9].iVertices.push_back(5);    faces[10].iVertices.push_back(6);   faces[11].iVertices.push_back(8);
        faces[9].iVertices.push_back(7);    faces[10].iVertices.push_back(7);   faces[11].iVertices.push_back(5);



    }


};

} //end of namespace PT

