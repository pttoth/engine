#include "engine/gl/Def.h"

#include "engine/gl/GlWrapper.h"
#include "engine/Utility.h"
#include "pt/logging.h"


using namespace engine;

void gl::Vertex::
SetPositionAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 3,
                             GL_FLOAT, gl::SKIP_TRANSPOSE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_POSITION );
}

void gl::Vertex::
SetTexelAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 2,
                             GL_FLOAT, gl::SKIP_TRANSPOSE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_TEXTURE );
}

void gl::Vertex::
SetNormalAttributePointer( GLuint index )
{
    gl::VertexAttribPointer( index, 3,
                             GL_FLOAT, gl::SKIP_TRANSPOSE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_NORMAL );
}


GLint gl::
MagFilterToGLint( MagFilter rule )
{
    switch( rule ){
    case gl::MagFilter::NEAREST:
        return GL_NEAREST;
    case gl::MagFilter::LINEAR:
        return GL_LINEAR;
//    default:
//        return GL_LINEAR;
    }

    PT_LOG_ERR( "MagFilterToGLint(): Unknown enum!" );
    PT_PRINT_DEBUG_STACKTRACE();
    return GL_LINEAR;
}


GLint gl::
MinFilterToGLint( MinFilter rule )
{
    switch( rule ){
    case MinFilter::LINEAR:
        return GL_LINEAR;                    // bilinear
    case MinFilter::NEAREST:
        return GL_NEAREST;
    case MinFilter::NEAREST_MIPMAP_NEAREST:
        return GL_NEAREST_MIPMAP_NEAREST;
    case MinFilter::NEAREST_MIPMAP_LINEAR:
        return GL_NEAREST_MIPMAP_LINEAR;
    case MinFilter::LINEAR_MIPMAP_NEAREST:
        return GL_LINEAR_MIPMAP_NEAREST;     // bilinear
    case MinFilter::LINEAR_MIPMAP_LINEAR:
        return GL_LINEAR_MIPMAP_LINEAR;      // trilinear
//    default:
//        return GL_NEAREST;
    };

    PT_LOG_ERR( "MinFilterToGLint(): Unknown enum!" );
    PT_PRINT_DEBUG_STACKTRACE();
    return GL_NEAREST;
}


GLint gl::
WrapRuleToGLint( WrapRule rule )
{
    switch( rule ){
    case gl::WrapRule::CLAMP_TO_EDGE:
        return GL_CLAMP_TO_EDGE;
    case gl::WrapRule::CLAMP_TO_BORDER:
        return GL_CLAMP_TO_BORDER;
    case gl::WrapRule::MIRRORED_REPEAT:
        return GL_MIRRORED_REPEAT;
    case gl::WrapRule::REPEAT:
        return GL_REPEAT;
    case gl::WrapRule::MIRROR_CLAMP_TO_EDGE:
        return GL_MIRROR_CLAMP_TO_EDGE;
//    default:
//        return GL_REPEAT;
    }

    PT_LOG_ERR( "WrapRuleToGLint(): Unknown enum!" );
    PT_PRINT_DEBUG_STACKTRACE();
    return GL_REPEAT;
}
