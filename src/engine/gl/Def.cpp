#include "engine/gl/Def.h"

#include "engine/gl/GlWrapper.h"

using namespace engine;

void gl::Vertex::
SetPositionAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 3,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_POSITION );
}

void gl::Vertex::
SetTexelAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 2,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_TEXTURE );
}

void gl::Vertex::
SetNormalAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 3,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_NORMAL );
}
