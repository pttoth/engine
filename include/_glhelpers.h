#pragma once

#include "GL/glew.h"    //has to include first :(
#include "GL/gl.h"
#include "engine/mathhelpers.hpp"

namespace pt{
namespace gl{

enum Normalization{
    SKIP_NORMALIZE = GL_FALSE,
    DO_NORMALIZE = GL_TRUE,
};

enum Transposition{
    SKIP_TRANSPOSE  = GL_FALSE,
    DO_TRANSPOSE    = GL_TRUE,
};

struct Vertex{
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
    #define VERTEX_OFFSET_POSITION reinterpret_cast<const GLvoid*>(0)
    #define VERTEX_OFFSET_TEXTURE  reinterpret_cast<const GLvoid*>(12)
    #define VERTEX_OFFSET_NORMAL   reinterpret_cast<const GLvoid*>(20)
#pragma GCC diagnostic warning "-Wzero-as-null-pointer-constant"

    math::float3 pos;
    math::float2 tex;
    math::float3 normal;

    Vertex(const math::float3& position     = math::float3(),
           const math::float2& texel        = math::float2(),
           const math::float3& normalvector = math::float3()
           ):   pos(position),
                tex(texel),
                normal(normalvector){
    }
};

//custom functions:
std::string GetErrorString(GLenum error);
std::string GetErrorDescription(GLenum error);
bool WasErrorGenerated();
bool WasErrorGeneratedAndPrint();
void UniformFloat3(GLint location, const math::float3& v);
void UniformFloat4x4(GLint location, GLboolean transpose, const math::float4x4& m);

//original functions:

//shaders
void ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
GLuint CreateShader(GLenum shaderType);
void CompileShader(GLuint shader);
void GetShaderiv(GLuint shader, GLenum pname, GLint *params);
void GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

void AttachShader(GLuint program,  GLuint shader);
void LinkProgram(GLuint program);
void GetProgramiv(GLuint program, GLenum pname, GLint *params);
void ValidateProgram(GLuint program);
void GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void UseProgram(GLuint program);




void Enable(GLenum cap);
void Disable(GLenum cap);

void EnableVertexAttribArray(GLuint index);
void DisableVertexAttribArray(GLuint index);


void ActiveTexture(GLenum texture);
void DeleteTextures(GLsizei n, const GLuint *textures);


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




} //gl
} //pttoth


