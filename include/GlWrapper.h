/** -----------------------------------------------------------------------------
  * FILE:    GlWrapper.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Provides a thread-safe wrapper for OpenGL functions.
  *          Performs error-checking after every function call.
  *          Provides helper classes and constants for OpenGL-related functionality.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "GL/glew.h"    //has to be included before 'gl.h'
#include "GL/gl.h"
#include "pt/math.h"

#include <mutex>


//TODO: remove this
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
    #define PT_GL_VERTEX_OFFSET_POSITION reinterpret_cast<const GLvoid*>(0)
    #define PT_GL_VERTEX_OFFSET_TEXTURE  reinterpret_cast<const GLvoid*>(12)
    #define PT_GL_VERTEX_OFFSET_NORMAL   reinterpret_cast<const GLvoid*>(20)
#pragma GCC diagnostic pop

namespace pt{
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
    pt::math::float3 pos;
    pt::math::float2 tex;
    pt::math::float3 normal;

    Vertex(const pt::math::float3& position     = pt::math::float3(),
           const pt::math::float2& texel        = pt::math::float2(),
           const pt::math::float3& normalvector = pt::math::float3()):
        pos(position),
        tex(texel),
        normal(normalvector)
    {}
};


//-------------------------
//  globals and constants
//-------------------------
extern std::mutex gMutex;


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
static const GLvoid* VertexOffsetPosition;
static const GLvoid* VertexOffsetTexture;
static const GLvoid* VertexOffsetNormal;
#pragma GCC diagnostic pop


//-------------------------
//  custom functions
//-------------------------

std::string GetErrorString(GLenum error);
std::string GetErrorDescription(GLenum error);
bool WasErrorGenerated();
bool WasErrorGeneratedAndPrint();
void UniformFloat3(GLint location, const pt::math::float3& v);
void UniformFloat4x4(GLint location, GLboolean transpose, const pt::math::float4x4& m);


//-------------------------
//  wrapped GL functions
//-------------------------

GLenum GetError(void);


//shaders
void ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
GLuint CreateShader(GLenum shaderType);
void DeleteShader(GLuint shader);

void CompileShader(GLuint shader);
void GetShaderiv(GLuint shader, GLenum pname, GLint *params);
void GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

void AttachShader(GLuint program,  GLuint shader);
void LinkProgram(GLuint program);
void GetProgramiv(GLuint program, GLenum pname, GLint *params);
void ValidateProgram(GLuint program);
void GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void UseProgram(GLuint program);

GLboolean IsShader(GLuint shader);
GLboolean IsProgram(GLuint program);

GLuint CreateProgram();
void DeleteProgram(GLuint program);


void FrontFace( GLenum mode);
void CullFace( GLenum mode);

void BlendFunc( GLenum sfactor, GLenum dfactor);
void BlendFunci( GLuint buf, GLenum sfactor, GLenum dfactor);



void Enable(GLenum cap);
void Disable(GLenum cap);

void EnableVertexAttribArray(GLuint index);
void DisableVertexAttribArray(GLuint index);


void ActiveTexture(GLenum texture);
void DeleteTextures(GLsizei n, const GLuint *textures);

void ClearColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void Clear( GLbitfield mask);

void GenVertexArrays(GLuint n, GLuint* arrays);


void GenBuffers(GLsizei n,  GLuint* buffers);
void DeleteBuffers(GLsizei n, const GLuint * buffers);

void GenTextures(GLsizei n, GLuint * textures);
void BindVertexArray(GLuint array);
void BindBuffer(GLenum target, GLuint buffer);
void BindTexture(GLenum target, GLuint texture);
void BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);

void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
void TexParameterf(GLenum target, GLenum pname, GLfloat param);
void TexParameteri(GLenum target, GLenum pname, GLint param);

//VertexAttribPointer
void VertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
void VertexAttribIPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);
void VertexAttribLPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);

//Drawing
void DrawArrays( GLenum mode, GLint first, GLsizei count);
void DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
//Uniform getters
GLint GetUniformLocation(GLuint program, const GLchar *name);

//Uniform setters
void Uniform1f(GLint location,  GLfloat v0);
void Uniform2f(GLint location,  GLfloat v0,  GLfloat v1);
void Uniform3f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2);
void Uniform4f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2,  GLfloat v3);
void Uniform1i(GLint location,  GLint v0);
void Uniform2i(GLint location,  GLint v0,  GLint v1);
void Uniform3i(GLint location,  GLint v0,  GLint v1,  GLint v2);
void Uniform4i(GLint location,  GLint v0,  GLint v1,  GLint v2,  GLint v3);
void Uniform1ui( GLint location, GLuint v0);
void Uniform2ui( GLint location, GLuint v0, GLuint v1);
void Uniform3ui( GLint location, GLuint v0, GLuint v1, GLuint v2);
void Uniform4ui( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void Uniform1fv( GLint location, GLsizei count, const GLfloat *value);
void Uniform2fv( GLint location, GLsizei count, const GLfloat *value);
void Uniform3fv( GLint location, GLsizei count, const GLfloat *value);
void Uniform4fv( GLint location, GLsizei count, const GLfloat *value);
void Uniform1iv( GLint location, GLsizei count, const GLint *value);
void Uniform2iv( GLint location, GLsizei count, const GLint *value);
void Uniform3iv( GLint location, GLsizei count, const GLint *value);
void Uniform4iv( GLint location, GLsizei count, const GLint *value);
void Uniform1uiv( GLint location, GLsizei count, const GLuint *value);
void Uniform2uiv( GLint location, GLsizei count, const GLuint *value);
void Uniform3uiv( GLint location, GLsizei count, const GLuint *value);
void Uniform4uiv( GLint location, GLsizei count, const GLuint *value);
void UniformMatrix2fv(GLint location,  GLsizei count,  GLboolean transpose, const GLfloat *value);
void UniformMatrix3fv(GLint location,  GLsizei count,  GLboolean transpose, const GLfloat *value);
void UniformMatrix4fv(GLint location,  GLsizei count,  GLboolean transpose, const GLfloat *value);
void UniformMatrix2x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void UniformMatrix3x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void UniformMatrix2x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void UniformMatrix4x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void UniformMatrix3x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
void UniformMatrix4x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);



void GetUniformfv(GLuint program, GLint location, GLfloat *params);
void GetUniformiv(GLuint program, GLint location, GLint *params);
void GetUniformuiv(GLuint program, GLint location, GLuint *params);
void GetUniformdv(GLuint program, GLint location, GLdouble *params);
void GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
void GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
void GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);


void GetBooleanv(GLenum pname, GLboolean* data);
void GetDoublev(GLenum pname, GLdouble* data);
void GetFloatv(GLenum pname, GLfloat* data);
void GetIntegerv(GLenum pname, GLint* data);
void GetInteger64v(GLenum pname, GLint64* data);
void GetBooleani_v(GLenum target, GLuint index, GLboolean* data);
void GetIntegeri_v(GLenum target, GLuint index, GLint* data);
void GetFloati_v(GLenum target, GLuint index, GLfloat* data);
void GetDoublei_v(GLenum target, GLuint index, GLdouble* data);
void GetInteger64i_v(GLenum target, GLuint index, GLint64* data);

const GLubyte* GetString(GLenum name);
const GLubyte* GetStringi(GLenum name, GLuint index);


} //end of namespace 'gl'

} //end of namespace 'pt'
