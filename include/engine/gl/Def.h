/** -----------------------------------------------------------------------------
  * FILE:    gl/Def.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Macros and definitions for project-wide use of GL.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "GL/glew.h"    //has to be included before 'gl.h'
#include "GL/gl.h"
#include "engine/Def.h"
#include "pt/math.h"

#ifndef ENGINE_DEBUG_NOAUTO
    #ifndef NDEBUG
        #define ENGINE_GL_DEBUG_ALL
    #endif
#endif


#ifdef ENGINE_GL_DEBUG_ALL
    #define ENGINE_GL_DEBUG_BUFFER
#endif


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

enum BufferHint{
    STREAM_DRAW     = GL_STREAM_DRAW,
    STREAM_READ     = GL_STREAM_READ,
    STREAM_COPY     = GL_STREAM_COPY,
    STATIC_DRAW     = GL_STATIC_DRAW,
    STATIC_READ     = GL_STATIC_READ,
    STATIC_COPY     = GL_STATIC_COPY,
    DYNAMIC_DRAW    = GL_DYNAMIC_DRAW,
    DYNAMIC_READ    = GL_DYNAMIC_READ,
    DYNAMIC_COPY    = GL_DYNAMIC_COPY,
};


enum BufferTarget{
    ARRAY_BUFFER                = GL_ARRAY_BUFFER,
    COPY_READ_BUFFER            = GL_COPY_READ_BUFFER,
    COPY_WRITE_BUFFER           = GL_COPY_WRITE_BUFFER,
    ELEMENT_ARRAY_BUFFER        = GL_ELEMENT_ARRAY_BUFFER,
    PIXEL_PACK_BUFFER           = GL_PIXEL_PACK_BUFFER,
    PIXEL_UNPACK_BUFFER         = GL_PIXEL_UNPACK_BUFFER,
    TEXTURE_BUFFER              = GL_TEXTURE_BUFFER,
    TRANSFORM_FEEDBACK_BUFFER   = GL_TRANSFORM_FEEDBACK_BUFFER,
    UNIFORM_BUFFER              = GL_UNIFORM_BUFFER,
};


enum DrawMode{
    POINTS                      = GL_POINTS,
    LINE_STRIP                  = GL_LINE_STRIP,
    LINE_LOOP                   = GL_LINE_LOOP,
    LINES                       = GL_LINES,
    LINE_STRIP_ADJACENCY        = GL_LINE_STRIP_ADJACENCY,
    LINES_ADJACENCY             = GL_LINES_ADJACENCY,
    TRIANGLE_STRIP              = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN                = GL_TRIANGLE_FAN,
    TRIANGLES                   = GL_TRIANGLES,
    TRIANGLE_STRIP_ADJACENCY    = GL_TRIANGLE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY         = GL_TRIANGLES_ADJACENCY,
    PATCHES                     = GL_PATCHES
};


enum Normalize{
    SKIP_NORMALIZE = GL_FALSE,
    DO_NORMALIZE   = GL_TRUE,
};


enum ShaderType{
    NO_SHADER_TYPE          = 0, // NO_SHADER_TYPE is custom, do not pass to GL calls!
    VERTEX_SHADER           = GL_VERTEX_SHADER,
    GEOMETRY_SHADER         = GL_GEOMETRY_SHADER,
    FRAGMENT_SHADER         = GL_FRAGMENT_SHADER,
    TESS_CONTROL_SHADER     = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION_SHADER  = GL_TESS_EVALUATION_SHADER,
    COMPUTE_SHADER          = GL_COMPUTE_SHADER,
};


enum Transpose{
    SKIP_TRANSPOSE  = GL_FALSE,
    DO_TRANSPOSE    = GL_TRUE,
};


struct Vertex{
    static void SetPositionAttributePointer( GLuint index );
    static void SetTexelAttributePointer( GLuint index );
    static void SetNormalAttributePointer( GLuint index );

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
