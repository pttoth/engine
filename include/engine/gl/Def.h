#pragma once

#include "GL/glew.h"    //has to be included before 'gl.h'
#include "GL/gl.h"
#include "pt/math.h"


//TODO: remove this?
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
    #define PT_GL_VERTEX_OFFSET_POSITION reinterpret_cast<const GLvoid*>(0)
    #define PT_GL_VERTEX_OFFSET_TEXTURE  reinterpret_cast<const GLvoid*>(12)
    #define PT_GL_VERTEX_OFFSET_NORMAL   reinterpret_cast<const GLvoid*>(20)
#pragma GCC diagnostic pop


namespace engine{
namespace gl{

//-------------------------
//  classes, structs, enums
//-------------------------

enum class Normalize{
    SKIP = GL_FALSE,
    DO   = GL_TRUE,
};


enum class Transpose{
    SKIP  = GL_FALSE,
    DO    = GL_TRUE,
};


struct Vertex{
    math::float3 pos;
    math::float2 texpos;
    math::float3 normal;

    Vertex(const math::float3& position     = math::float3(),
           const math::float2& texel        = math::float2(),
           const math::float3& normalvector = math::float3()):
        pos( position ),
        texpos( texel ),
        normal( normalvector )
    {}
};

} // end of namespace 'gl'
} // end of namespace 'engine'
