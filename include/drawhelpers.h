#pragma once

//#include "engine/mathhelpers.hpp"
#include "engine/glhelpers.h"

#include <vector>
#include <assert.h>

namespace pt{

static const    math::float3    color_black             = math::float3(0.0f,0.0f,0.0f);
static const    math::float3    color_white             = math::float3(1.0f,1.0f,1.0f);
static const    math::float3    color_red               = math::float3(1.0f,0.0f,0.0f);
static const    math::float3    color_green             = math::float3(0.0f,1.0f,0.0f);
static const    math::float3    color_blue              = math::float3(0.0f,0.0f,1.0f);
static const    math::float3    color_yellow            = math::float3(1.0f,1.0f,0.0f);
static const    math::float3    color_teal              = math::float3(0.0f,1.0f,1.0f);
static const    math::float3    color_purple            = math::float3(1.0f,0.0f,1.0f);


inline void
GenerateNormals( std::vector<gl::Vertex>& vertices,
                      const std::vector<math::int3>& vertex_indices){
    std::vector<std::vector<math::float3>> normals; //a normal array for each vertex
    normals.resize(vertices.size());

    math::float3 current_normal;
    math::float3 A,B,C; // triangle vertices
    math::float3 na,nc; // triangle sides (normalized)
    uint32_t iA, iB, iC;
    for(const math::int3& current_triangle : vertex_indices){
        assert(-1 < current_triangle.x);
        assert(-1 < current_triangle.y);
        assert(-1 < current_triangle.z);
        iA = static_cast<uint32_t>(current_triangle.x);
        iB = static_cast<uint32_t>(current_triangle.y);
        iC = static_cast<uint32_t>(current_triangle.z);
        A = vertices[iA].pos;
        B = vertices[iB].pos;
        C = vertices[iC].pos;
        nc = (B-A).normalize();
        na = (C-B).normalize();
        current_normal = (nc).cross(na);
        normals[iA].push_back(current_normal);
        normals[iB].push_back(current_normal);
        normals[iC].push_back(current_normal);
    }

    //...at this point, all vertices have all normals available
    //generate their average per vertex
    for(size_t j=0; j<normals.size(); ++j){
        math::float3 avgnormal;
        for(size_t i=0; i<normals[j].size(); ++i){
            avgnormal += normals[j][i];
        }
        avgnormal = avgnormal.normalize();
        vertices[j].normal = avgnormal;
    }
}

}

