#pragma once

#include <vector>

#include "engine/glhelpers.h"
//#include "engine/mathhelpers.hpp"
#include "engine/camera.h"


namespace pt{

class Cube{
    GLuint          vao;
    GLuint          vbo[6];
    math::float3    pos;
    float           orient;
    math::float4    modelTF;

    bool m_initialized;

    //note: wastes memory with gl::Vertex instead of math::float3 (faster to copy this way)
    std::vector< std::vector<gl::Vertex> > _createVertices() const;
public:
    GLuint          ShaderProgram;
    const char*     shNameColor;
    const char*     shNameMVP;
    const char*     shNameUseFixColor;
    const char*     shNameGSampler;
    engine::Camera* cam;


    Cube();
    virtual ~Cube();

    void createRenderContext();
    void destroyRenderContext();
    void draw();
};

}



