#include "engine/gl/GlWrapper.h"

#include "pt/logging.h"

#include <assert.h>
#include <iostream>
#include <sstream>

using namespace engine;

std::mutex gl::mutex_gl;

//--------------------------------------------------
//hidden inner functions

bool
WasErrorGenerated_NoLock()
{
    return GL_NO_ERROR != glGetError();
}


bool
WasErrorGeneratedAndPrint_NoLock()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ){
        PT_LOG_ERR( "\nOpenGL error: "  << gl::GetErrorString(error) );
        PT_LOG_ERR( " Description:  "   << gl::GetErrorDescription(error) );
    }
    return GL_NO_ERROR != error;
}


void
GetProgramiv_NoLock( GLuint program, GLenum pname, GLint* params )
{
    glGetProgramiv(program, pname, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


//--------------------------------------------------
//custom functions:


std::string gl::
GetErrorDescription( GLenum error )
{
    switch (error){
    case GL_NO_ERROR:
        return "No error has been recorded.\n    The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument.\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
        return "A numeric argument is out of range.\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state.\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "The framebuffer object is not complete.\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command.\n    The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        return "Unknown error. No description available.";
    }
}


std::string gl::
GetErrorString( GLenum error )
{
    switch (error){
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    default:
        return "UNKNOWN_GL_ERROR_ENUM";
    }
}


std::string gl::
ToString( GLenum value )
{
    switch (value){
    case GL_RGBA:               return "GL_RGBA";
    case GL_RGBA8:              return "GL_RGBA8";
    case GL_DEPTH_COMPONENT:    return "GL_DEPTH_COMPONENT";

    //TODO: fill...
    }

    PT_LOG_WARN( "Unknown texture format: " << value );
    return "n/a";
}


void gl::
PrintShaderInfoLog( GLint handle )
{
    GLchar  infoLog[ 16*1024 ];
    GLsizei msgLength = 0;
    infoLog[0] = 0;
    gl::GetShaderInfoLog( handle, sizeof(infoLog), &msgLength, infoLog );
    PT_LOG_ERR( "-----Shader'" << handle << "' Info log (length: " << msgLength << ")-----\n" << infoLog << "'\n--------------------------------------" );
}


void gl::
PrintProgramInfoLog( GLint handle )
{
    GLchar  infoLog[ 16*1024 ];
    GLsizei msgLength = 0;
    infoLog[0] = 0;
    gl::GetProgramInfoLog( handle, sizeof(infoLog), &msgLength, infoLog );
    PT_LOG_ERR( "-----ShaderProgram'" << handle << "' Info log (length: " << msgLength << "): '" << infoLog << "'\n--------------------------------------" );
}


void gl::
UniformFloat3( GLint location, const math::float3& v )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glUniform3fv( location, 1, v.v );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformFloat4x4( GLint location, GLboolean transpose, const math::float4x4& m )
{
    gl::UniformMatrix4fv(location, 1, transpose, &(m.m[0][0]));
}


bool gl::
WasErrorGenerated()
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    return WasErrorGenerated_NoLock();
}


bool gl::
WasErrorGeneratedAndPrint()
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    return WasErrorGeneratedAndPrint_NoLock();
}


std::string gl::
GetBufferTargetAsString( gl::BufferTarget target )
{
    switch( target ){
    case gl::BufferTarget::ARRAY_BUFFER:                return std::string( "ARRAY_BUFFER" );
    case gl::BufferTarget::COPY_READ_BUFFER:            return std::string( "COPY_READ_BUFFER" );
    case gl::BufferTarget::COPY_WRITE_BUFFER:           return std::string( "COPY_WRITE_BUFFER" );
    case gl::BufferTarget::ELEMENT_ARRAY_BUFFER:        return std::string( "ELEMENT_ARRAY_BUFFER" );
    case gl::BufferTarget::PIXEL_PACK_BUFFER:           return std::string( "PIXEL_PACK_BUFFER" );
    case gl::BufferTarget::PIXEL_UNPACK_BUFFER:         return std::string( "PIXEL_UNPACK_BUFFER" );
    case gl::BufferTarget::TEXTURE_BUFFER:              return std::string( "TEXTURE_BUFFER" );
    case gl::BufferTarget::TRANSFORM_FEEDBACK_BUFFER:   return std::string( "TRANSFORM_FEEDBACK_BUFFER" );
    case gl::BufferTarget::UNIFORM_BUFFER:              return std::string( "UNIFORM_BUFFER" );
    }
    PT_LOG_ERR( "GetBufferTargetAsString(): Could not identify 'target'(" << target << ")" );
    assert( false );
    return std::string();
}


std::string gl::
GetShaderTypeAsString( ShaderType type )
{
    switch( type ){
    case gl::ShaderType::COMPUTE_SHADER:
        return std::string( "Compute Shader" );
    case gl::ShaderType::FRAGMENT_SHADER:
        return std::string( "Fragment Shader" );
    case gl::ShaderType::GEOMETRY_SHADER:
        return std::string( "Geometry Shader" );
    case gl::ShaderType::VERTEX_SHADER:
        return std::string( "Vertex Shader" );
    case gl::ShaderType::TESS_CONTROL_SHADER:
        return std::string( "Tessellation Control Shader" );
    case gl::ShaderType::TESS_EVALUATION_SHADER:
        return std::string( "Tessellation Evaluation Shader" );
    case gl::ShaderType::NO_SHADER_TYPE:
        return std::string( "NONE (default)" );
    default:
        std::stringstream ss;
        ss << "UNKNOWN (" << type << ")";
        return ss.str();
    }
    assert( false );
    return std::string();
}


void gl::
ClearColor( const math::float4& v )
{
    gl::ClearColor( v.x, v.y, v.z, v.w );
}


void gl::
UnbindBuffer( GLenum target )
{
    gl::BindBuffer( target, 0 );
}


void gl::
UnbindTexture( GLenum target )
{
    gl::BindTexture( target, 0 );
}


//--------------------------------------------------
//original functions:

void gl::
ActiveTexture(GLenum texture)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glActiveTexture(texture);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
AttachShader(GLuint program, GLuint shader)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glAttachShader(program, shader);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


//Note: see gl::Buffer class(es) for templated overloads of this function
void gl::
BindBuffer(GLenum target, GLuint buffer)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBindBuffer(target, buffer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BindBufferBase( GLenum target, GLuint index, GLuint buffer )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBindBufferBase( target, index, buffer );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BindFramebuffer( GLenum target, GLuint framebuffer )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBindFramebuffer( target, framebuffer );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BindTexture(GLenum target, GLuint texture)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBindTexture(target, texture);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BindVertexArray(GLuint array)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBindVertexArray(array);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BlendFunc(GLenum sfactor, GLenum dfactor)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBlendFunc( sfactor, dfactor );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BlendFunci(GLuint buf, GLenum sfactor, GLenum dfactor)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBlendFunci( buf, sfactor, dfactor );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glBufferData(target, size, data, usage);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Clear(GLbitfield mask)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glClear( mask );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
ClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glClearColor( red, green, blue, alpha );
}


void gl::
CompileShader(GLuint shader)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glCompileShader(shader);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}



void gl::
CopyTexImage2D( GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glCopyTexImage2D( target, level, internalformat, x, y, width, height, border );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLuint gl::
CreateProgram()
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    GLuint retval = glCreateProgram();
    assert( 0 != retval );
    return retval;
}


GLuint gl::
CreateShader(GLenum shaderType)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    GLuint retval = glCreateShader(shaderType);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


void gl::
CullFace(GLenum mode)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glCullFace( mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DeleteBuffers(GLsizei n, const GLuint *buffers)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDeleteBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DeleteProgram(GLuint program)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDeleteProgram( program );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DeleteShader(GLuint shader)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDeleteShader( shader );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DeleteTextures(GLsizei n, const GLuint *textures)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDeleteTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Disable(GLenum cap)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDisable(cap);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DisableVertexAttribArray(GLuint index)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDisableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DrawArrays(GLenum mode, GLint first, GLsizei count)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDrawArrays(mode, first, count);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DrawBuffer( GLenum buf )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glDrawBuffer( buf );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glDrawElements(mode, count, type, indices);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Enable(GLenum cap)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glEnable(cap);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
EnableVertexAttribArray(GLuint index)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glEnableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
FramebufferTexture2D( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glFramebufferTexture2D( target, attachment, textarget, texture, level );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
FrontFace(GLenum mode)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glFrontFace( mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GenBuffers(GLsizei n, GLuint *buffers)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GenerateMipmap( GLenum target )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenerateMipmap( target );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GenerateTextureMipmap( GLuint texture )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenerateTextureMipmap( texture );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GenFramebuffers( GLsizei n, GLuint* ids )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenFramebuffers( n, ids );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GenTextures(GLsizei n, GLuint *textures)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


#pragma GCC diagnostic ignored "-Wsign-conversion"
void gl::
GenVertexArrays(GLuint n, GLuint *arrays)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGenVertexArrays(n, arrays);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}
#pragma GCC diagnostic warning "-Wsign-conversion"


void gl::
GetActiveUniformsiv( GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetActiveUniformsiv( program, uniformCount, uniformIndices, pname, params );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetBooleani_v(GLenum target, GLuint index, GLboolean *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetBooleani_v(target, index, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetBooleanv(GLenum pname, GLboolean *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetBooleanv(pname, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetDoublei_v(GLenum target, GLuint index, GLdouble *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetDoublei_v(target, index, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetDoublev(GLenum pname, GLdouble *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetDoublev(pname, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLenum gl::
GetError()
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    return glGetError();
}


void gl::
GetFloati_v(GLenum target, GLuint index, GLfloat *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetFloati_v(target, index, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetFloatv(GLenum pname, GLfloat *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetFloatv(pname, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetIntegeri_v(GLenum target, GLuint index, GLint *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetIntegeri_v(target, index, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetIntegerv(GLenum pname, GLint *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetIntegerv(pname, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetInteger64i_v(GLenum target, GLuint index, GLint64 *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetInteger64i_v(target, index, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetInteger64v(GLenum pname, GLint64 *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetInteger64v(pname, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetnUniformdv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetnUniformfv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetnUniformiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetnUniformuiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetProgramInfoLog(program, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    GetProgramiv_NoLock(program, pname, params);
}


void gl::
GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetShaderiv(shader, pname, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


const GLubyte *gl::
GetString(GLenum name)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    const GLubyte* retval = glGetString( name );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


const GLubyte *gl::
GetStringi(GLenum name, GLuint index)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    const GLubyte* retval = glGetStringi( name, index );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


GLuint gl::
GetUniformBlockIndex( GLuint program, const GLchar* uniformBlockName )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    GLuint retval = glGetUniformBlockIndex( program, uniformBlockName );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


void gl::
GetUniformdv(GLuint program, GLint location, GLdouble *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetUniformdv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}

void gl::
GetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetUniformfv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetUniformiv(GLuint program, GLint location, GLint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetUniformiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
GetUniformIndices( GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetUniformIndices( program, uniformCount, uniformNames, uniformIndices );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLint gl::
GetUniformLocation(GLuint program, const GLchar *name)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    GLint retval = glGetUniformLocation( program, name );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


void gl::
GetUniformuiv(GLuint program, GLint location, GLuint *params)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glGetUniformuiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Hint( GLenum target, GLenum mode )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glHint( target, mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLboolean gl::
IsShader(GLuint shader)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    return glIsShader( shader );
}


GLboolean gl::
IsProgram(GLuint program)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    return glIsProgram( program );
}


void gl::
LineWidth( GLfloat width )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glLineWidth( width );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
LinkProgram(GLuint program)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glLinkProgram(program);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glNamedBufferData(buffer, size, data, usage);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
NamedFramebufferDrawBuffer( GLuint framebuffer, GLenum buf )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glNamedFramebufferDrawBuffer( framebuffer, buf );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
NamedFramebufferReadBuffer( GLuint framebuffer, GLenum mode )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glNamedFramebufferReadBuffer( framebuffer, mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
PolygonMode( GLenum face, GLenum mode )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glPolygonMode( face, mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
ReadBuffer( GLenum mode )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glReadBuffer( mode );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
ReadPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* data )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glReadPixels( x, y, width, height, format, type, data );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
ReadnPixels( GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data )
{
    std::lock_guard<std::mutex> lock( mutex_gl );
    glReadnPixels( x, y, width, height, format, type, bufSize, data );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glShaderSource(shader, count, string, length);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glTexParameterf(target, pname, param);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
TexParameteri(GLenum target, GLenum pname, GLint param)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glTexParameteri(target, pname, param);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformBlockBinding( GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding )
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformBlockBinding( program, uniformBlockIndex, uniformBlockBinding );
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform1f(GLint location, GLfloat v0)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1f(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2f(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3f(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4f(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}

void gl::
Uniform1i(GLint location, GLint v0)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1i(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2i(GLint location, GLint v0, GLint v1)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2i(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3i(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4i(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform1ui(GLint location, GLuint v0)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1ui(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2ui(GLint location, GLuint v0, GLuint v1)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2ui(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3ui(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4ui(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform1uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform2uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform3uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniform4uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}



void gl::
UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix2x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix2x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix3x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix3x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix4x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUniformMatrix4x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
UseProgram(GLuint program)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glUseProgram(program);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


bool gl::
ValidateProgram(GLuint program)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glValidateProgram(program);
    assert( !WasErrorGeneratedAndPrint_NoLock() );

    int program_validated;
    GetProgramiv_NoLock(program, GL_VALIDATE_STATUS, &program_validated);
    if( !program_validated ){
        PT_LOG_ERR( "OpenGL ERROR: could not validate ShaderProgram!" );
    }
    assert( program_validated );
    return program_validated;
}


void gl::
VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glVertexAttribIPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glVertexAttribLPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void gl::
VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    std::lock_guard<std::mutex> lock(mutex_gl);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}
