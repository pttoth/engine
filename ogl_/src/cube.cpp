#include "cube.h"

#include "engine/drawhelpers.h"

using namespace pt;


std::vector< std::vector<gl::Vertex> > Cube::
_createVertices() const{
    std::vector< std::vector<gl::Vertex> > retval;
    std::vector<gl::Vertex> vertices;
    //right plane (red)
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    //front plane (green)
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,  -20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    //top plane (blue)
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,   20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    //left plane (yellow)
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,  -20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    //back plane (teal)
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,   20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,  -20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    //bottom plane (purple)
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3( -20.0f,  -20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,   20.0f,  -20.0f ) ) );
    vertices.push_back( gl::Vertex(math::float3(  20.0f,  -20.0f,  -20.0f ) ) );
    retval.push_back(vertices);
    vertices.clear();
    return retval;
}

Cube::
Cube(){
    ShaderProgram = 0;
    shNameColor = nullptr;
    shNameMVP = nullptr;
    shNameUseFixColor = nullptr;
    shNameGSampler = nullptr;
    cam = nullptr;
}

Cube::
~Cube(){
    destroyRenderContext();
}

#pragma GCC diagnostic ignored "-Wsign-conversion"
void Cube::
createRenderContext(){
    //todo:     clean up
    //...

    std::vector<std::vector<gl::Vertex>> vertices = _createVertices();
//    gl::GenVertexArrays(1, &vao);
//    gl::BindVertexArray(vao);
    gl::GenBuffers(6, &vbo[0]);
    for(size_t i=0; i<6; ++i){
        gl::BindBuffer(GL_ARRAY_BUFFER, vbo[i]);
        gl::BufferData(GL_ARRAY_BUFFER,
                         vertices[i].size()*sizeof(gl::Vertex),
                         vertices[i].data(),
                         GL_STATIC_DRAW);
    }
    /*
    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0, 3, GL_FLOAT, gl::SKIP_NORMALIZE, sizeof(gl::Vertex), VERTEX_OFFSET_POSITION );
*/
    m_initialized = true;
}
#pragma GCC diagnostic warning "-Wsign-conversion"

void Cube::
destroyRenderContext(){
    if(m_initialized){
        gl::DeleteTextures(6, &vbo[0]);
        m_initialized = false;
    }
}

void Cube::
draw(){
    if(m_initialized){

        GLint uniMVP            = gl::GetUniformLocation(ShaderProgram, shNameMVP);
        GLint uniUseFixColor    = gl::GetUniformLocation(ShaderProgram, shNameUseFixColor);
        GLint uniColor          = gl::GetUniformLocation(ShaderProgram, shNameColor);
        GLint uniGSampler       = gl::GetUniformLocation(ShaderProgram, shNameGSampler);

        math::float4x4 MVP;
        math::float4x4 M;
        math::float4x4 V;
        math::float4x4 P;

        assert(cam != nullptr);

        //Model matrix
        math::float4x4 Mtrans = math::float4x4::identity;
        math::float4x4 Mrot = math::float4x4::identity;
        math::float4x4 Mscale = math::float4x4::identity;
        M = Mscale * Mrot * Mtrans;
        V = cam->getViewMtx();
        P = cam->getProjMtx();
        MVP = M * V * P;

        gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(MVP.m[0][0]));
        gl::Uniform1i(uniGSampler, 0);
        gl::Uniform1i(uniUseFixColor, 1);

        std::vector<math::float3> colors;
        colors.push_back(color_red);
        colors.push_back(color_green);
        colors.push_back(color_blue);
        colors.push_back(color_yellow);
        colors.push_back(color_teal);
        colors.push_back(color_purple);

        gl::EnableVertexAttribArray(0);
        for(size_t i=0; i<6; ++i){
            gl::BindBuffer(GL_ARRAY_BUFFER, vbo[i]);
            gl::UniformFloat3(uniColor, colors[i]);
            gl::VertexAttribPointer(0, 3, GL_FLOAT, gl::SKIP_NORMALIZE, sizeof(gl::Vertex), VERTEX_OFFSET_POSITION );
            gl::DrawArrays(GL_TRIANGLES, 0, 6);
        }

        gl::Uniform1i(uniUseFixColor, 0);
    }
}
