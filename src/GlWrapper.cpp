#include "GlWrapper.h"

#include <assert.h>
#include <iostream>

//--------------------------------------------------
//hidden inner functions

bool
WasErrorGenerated_NoLock()
{
    return GL_NO_ERROR != glGetError();
}


bool WasErrorGeneratedAndPrint_NoLock()
{
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ){
        std::cout << "\nOpenGL error: " << gl::GetErrorString(error) << "\n";
        std::cout << " Description:  " << gl::GetErrorDescription(error) << "\n";
    }
    return GL_NO_ERROR != error;
}


//--------------------------------------------------
//custom functions:

std::string pt::gl::
GetErrorString(GLenum error)
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


std::string pt::gl::
GetErrorDescription(GLenum error)
{
    switch (error){
    case GL_NO_ERROR:
        return "No error has been recorded.\r\n    The value of this symbolic constant is guaranteed to be 0.";
    case GL_INVALID_ENUM:
        return "An unacceptable value is specified for an enumerated argument.\r\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:
        return "A numeric argument is out of range.\r\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:
        return "The specified operation is not allowed in the current state.\r\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "The framebuffer object is not complete.\r\n    The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:
        return "There is not enough memory left to execute the command.\r\n    The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    case GL_STACK_UNDERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
    case GL_STACK_OVERFLOW:
        return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
    default:
        return "Unknown error. No description available.";
    }
}


bool pt::gl::
WasErrorGenerated()
{
    std::lock_guard<std::mutex> lock(gMutex);
    return WasErrorGenerated_NoLock();
}


bool pt::gl::
WasErrorGeneratedAndPrint()
{
    std::lock_guard<std::mutex> lock(gMutex);
    return WasErrorGeneratedAndPrint_NoLock();
}


void pt::gl::
UniformFloat3(GLint location, const math::float3& v)
{

    std::lock_guard<std::mutex> lock(gMutex);
    glUniform3fv(location, 1, v.v);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformFloat4x4(GLint location, GLboolean transpose, const math::float4x4 &m)
{
    gl::UniformMatrix4fv(location, 1, transpose, &(m.m[0][0]));
}


//--------------------------------------------------
//original functions:
void pt::gl::
Enable(GLenum cap)
{
    std::lock_guard<std::mutex> lock(gMutex);
    glEnable(cap);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Disable(GLenum cap)
{
    std::lock_guard<std::mutex> lock(gMutex);
    glDisable(cap);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
EnableVertexAttribArray(GLuint index)
{
    std::lock_guard<std::mutex> lock(gMutex);
    glEnableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
DisableVertexAttribArray(GLuint index)
{
    std::lock_guard<std::mutex> lock(gMutex);
    glDisableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
ActiveTexture(GLenum texture)
{
    glActiveTexture(texture);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
DeleteTextures(GLsizei n, const GLuint *textures)
{
    glDeleteTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


#pragma GCC diagnostic ignored "-Wsign-conversion"
void pt::gl::
GenVertexArrays(GLuint n, GLuint *arrays)
{
    glGenVertexArrays(n, arrays);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}
#pragma GCC diagnostic warning "-Wsign-conversion"


void pt::gl::
GenBuffers(GLsizei n, GLuint *buffers)
{
    glGenBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
DeleteBuffers(GLsizei n, const GLuint *buffers)
{
    glDeleteBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
BindVertexArray(GLuint array)
{
    glBindVertexArray(array);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
BindBuffer(GLenum target, GLuint buffer)
{
    glBindBuffer(target, buffer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)
{
    glBufferData(target, size, data, usage);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)
{
    glNamedBufferData(buffer, size, data, usage);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glVertexAttribIPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    glVertexAttribLPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
DrawArrays(GLenum mode, GLint first, GLsizei count)
{
    glDrawArrays(mode, first, count);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
{
    glDrawElements(mode, count, type, indices);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
BindTexture(GLenum target, GLuint texture)
{
    glBindTexture(target, texture);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GenTextures(GLsizei n, GLuint *textures)
{
    glGenTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
    glTexParameterf(target, pname, param);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
TexParameteri(GLenum target, GLenum pname, GLint param)
{
    glTexParameteri(target, pname, param);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1f(GLint location, GLfloat v0)
{
    glUniform1f(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2f(GLint location, GLfloat v0, GLfloat v1)
{
    glUniform2f(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1i(GLint location, GLint v0)
{
    glUniform1i(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2i(GLint location, GLint v0, GLint v1)
{
    glUniform2i(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
{
    glUniform3i(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
    glUniform4i(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1ui(GLint location, GLuint v0)
{
    glUniform1ui(location, v0);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2ui(GLint location, GLuint v0, GLuint v1)
{
    glUniform2ui(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
    glUniform3ui(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    glUniform4ui(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform1fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform2fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform3fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
    glUniform4fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
    glUniform1iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
    glUniform2iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
    glUniform3iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
    glUniform4iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
    glUniform1uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
    glUniform2uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
    glUniform3uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
    glUniform4uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix2x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix3x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix2x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix3x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLint pt::gl::
GetUniformLocation(GLuint program, const GLchar *name)
{
    GLint retval = glGetUniformLocation( program, name );
    //-------
    //TODO: remove this
    bool result = false;
    result = !WasErrorGeneratedAndPrint_NoLock();
    if(!result){
        assert(false);
    }
    //------
    //assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}





GLenum pt::gl::
GetError()
{
    std::lock_guard<std::mutex> lock(gMutex);
    return glGetError();
}







void pt::gl::
ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
    glShaderSource(shader, count, string, length);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


GLuint pt::gl::
CreateShader(GLenum shaderType)
{
    GLuint retval = glCreateShader(shaderType);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
    return retval;
}


void pt::gl::
CompileShader(GLuint shader)
{
    glCompileShader(shader);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetShaderiv(GLuint shader, GLenum pname, GLint *params)
{
    glGetShaderiv(shader, pname, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
AttachShader(GLuint program, GLuint shader)
{
    glAttachShader(program, shader);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
LinkProgram(GLuint program)
{
    glLinkProgram(program);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetProgramiv(GLuint program, GLenum pname, GLint *params)
{
    glGetProgramiv(program, pname, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}

void pt::gl::
ValidateProgram(GLuint program)
{
    glValidateProgram(program);
    bool success = !WasErrorGeneratedAndPrint_NoLock();
    int program_validated;
    pt::gl::GetProgramiv(program, GL_VALIDATE_STATUS, &program_validated);
    if( !program_validated ){
        std::cout << "OpenGL ERROR: could not validate ShaderProgram!\n";
    }
    assert( program_validated );
    assert( success );
}

void pt::gl::
GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog)
{
    glGetProgramInfoLog(program, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}

void pt::gl::
UseProgram(GLuint program)
{
    glUseProgram(program);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}



void pt::gl::
GetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    glGetUniformfv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetUniformiv(GLuint program, GLint location, GLint *params)
{
    glGetUniformiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetUniformuiv(GLuint program, GLint location, GLuint *params)
{
    glGetUniformuiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetUniformdv(GLuint program, GLint location, GLdouble *params)
{
    glGetUniformdv(program, location, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params)
{
    glGetnUniformfv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
    glGetnUniformiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params)
{
    glGetnUniformuiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}


void pt::gl::
GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params)
{
    glGetnUniformdv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint_NoLock() );
}
