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

#include "engine/gl/Def.h"
#include "pt/macros.h"

#include <mutex>

#ifdef ENGINE_GL_DEBUG_ALL
#define ENGINE_GL_UNBIND_ENABLED
#endif

#ifdef ENGINE_GL_UNBIND_ENABLED
//#define GL_UnbindBuffer( target )  glBindBuffer( target, 0 )
#define GL_UnbindBuffer( target )  engine::gl::UnbindBuffer( target )
#define GL_UnbindTexture( target )  engine::gl::UnbindTexture( target )
#else
#define GL_UnbindBuffer( target )  (__PT_VOID_CAST (0))
#define GL_UnbindTexture( target )  (__PT_VOID_CAST (0))
#endif

namespace engine{
namespace gl{

//-------------------------
//  globals and constants
//-------------------------

extern std::mutex mutex_gl;

//-------------------------
//  custom functions
//-------------------------

std::string GetErrorDescription( GLenum error );
std::string GetErrorString( GLenum error );

void PrintShaderInfoLog( GLint handle );
void PrintProgramInfoLog( GLint handle );

void UniformFloat3( GLint location, const math::float3& v );
void UniformFloat4x4( GLint location, GLboolean transpose, const math::float4x4& m );

bool WasErrorGenerated();
bool WasErrorGeneratedAndPrint();

std::string GetBufferTargetAsString( gl::BufferTarget target );
std::string GetShaderTypeAsString( gl::ShaderType type );
void ClearColor( const math::float4& v );

void UnbindBuffer( GLenum target );
void UnbindTexture( GLenum target );

//-------------------------
//  wrapped GL functions
//-------------------------



//shaders

void ActiveTexture(GLenum texture);
void AttachShader(GLuint program,  GLuint shader);
void BindBuffer(GLenum target, GLuint buffer);
void BindTexture(GLenum target, GLuint texture);
void BindVertexArray(GLuint array);
void BlendFunc( GLenum sfactor, GLenum dfactor);
void BlendFunci( GLuint buf, GLenum sfactor, GLenum dfactor);
void BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
void Clear( GLbitfield mask);
void ClearColor( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void CompileShader(GLuint shader);
GLuint CreateProgram();
GLuint CreateShader(GLenum shaderType);
void CullFace( GLenum mode);
void DeleteBuffers(GLsizei n, const GLuint * buffers);
void DeleteProgram(GLuint program);
void DeleteShader(GLuint shader);
void DeleteTextures(GLsizei n, const GLuint *textures);
void Disable(GLenum cap);
void DisableVertexAttribArray(GLuint index);
void DrawArrays( GLenum mode, GLint first, GLsizei count);
void DrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid* indices );
void Enable(GLenum cap);
void EnableVertexAttribArray(GLuint index);
void FrontFace( GLenum mode);
void GenBuffers(GLsizei n,  GLuint* buffers);
void GenTextures(GLsizei n, GLuint * textures);
void GenVertexArrays(GLuint n, GLuint* arrays);
void GetBooleani_v(GLenum target, GLuint index, GLboolean* data);
void GetBooleanv(GLenum pname, GLboolean* data);
void GetDoublei_v(GLenum target, GLuint index, GLdouble* data);
void GetDoublev(GLenum pname, GLdouble* data);
GLenum GetError(void);
void GetFloati_v(GLenum target, GLuint index, GLfloat* data);
void GetFloatv(GLenum pname, GLfloat* data);
void GetIntegeri_v(GLenum target, GLuint index, GLint* data);
void GetIntegerv(GLenum pname, GLint* data);
void GetInteger64i_v(GLenum target, GLuint index, GLint64* data);
void GetInteger64v(GLenum pname, GLint64* data);
void GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
void GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
void GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params);
void GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
void GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void GetProgramiv(GLuint program, GLenum pname, GLint *params);
void GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
void GetShaderiv(GLuint shader, GLenum pname, GLint *params);
const GLubyte* GetString(GLenum name);
const GLubyte* GetStringi(GLenum name, GLuint index);
void GetUniformdv(GLuint program, GLint location, GLdouble *params);
void GetUniformfv(GLuint program, GLint location, GLfloat *params);
void GetUniformiv(GLuint program, GLint location, GLint *params);
GLint GetUniformLocation(GLuint program, const GLchar *name);
void GetUniformuiv(GLuint program, GLint location, GLuint *params);
void Hint( GLenum target, GLenum mode );
GLboolean IsShader(GLuint shader);
GLboolean IsProgram(GLuint program);
void LineWidth( GLfloat width );
void LinkProgram(GLuint program);
void NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
void PolygonMode( GLenum face, GLenum mode );
void ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
void TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
void TexParameterf(GLenum target, GLenum pname, GLfloat param);
void TexParameteri(GLenum target, GLenum pname, GLint param);

void Uniform1f( GLint location, GLfloat v0 );
void Uniform2f( GLint location, GLfloat v0, GLfloat v1 );
void Uniform3f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 );
void Uniform4f( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 );
void Uniform1fv( GLint location, GLsizei count, const GLfloat* value );
void Uniform2fv( GLint location, GLsizei count, const GLfloat* value );
void Uniform3fv( GLint location, GLsizei count, const GLfloat* value );
void Uniform4fv( GLint location, GLsizei count, const GLfloat* value );
void Uniform1i( GLint location, GLint v0 );
void Uniform2i( GLint location, GLint v0, GLint v1 );
void Uniform3i( GLint location, GLint v0, GLint v1, GLint v2 );
void Uniform4i( GLint location, GLint v0, GLint v1, GLint v2, GLint v3 );
void Uniform1iv( GLint location, GLsizei count, const GLint* value );
void Uniform2iv( GLint location, GLsizei count, const GLint* value );
void Uniform3iv( GLint location, GLsizei count, const GLint* value );
void Uniform4iv( GLint location, GLsizei count, const GLint* value );
void Uniform1ui( GLint location, GLuint v0 );
void Uniform2ui( GLint location, GLuint v0, GLuint v1 );
void Uniform3ui( GLint location, GLuint v0, GLuint v1, GLuint v2 );
void Uniform4ui( GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3 );
void Uniform1uiv( GLint location, GLsizei count, const GLuint* value );
void Uniform2uiv( GLint location, GLsizei count, const GLuint* value );
void Uniform3uiv( GLint location, GLsizei count, const GLuint* value );
void Uniform4uiv( GLint location, GLsizei count, const GLuint* value );

void UniformMatrix2fv(   GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix2x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix2x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix3fv(   GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix3x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix3x4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix4fv(   GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix4x2fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UniformMatrix4x3fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat* value );
void UseProgram( GLuint program );
bool ValidateProgram( GLuint program );
void VertexAttribIPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
void VertexAttribLPointer( GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer );
void VertexAttribPointer( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer );


} //end of namespace 'gl'

} //end of namespace 'engine'
