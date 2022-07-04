
#include "engine/glhelpers.h"

#include <assert.h>
#include <iostream>

using namespace pt;

//--------------------------------------------------
//custom functions:

std::string gl::
GetErrorString(GLenum error){
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
GetErrorDescription(GLenum error){
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

bool gl::
WasErrorGenerated(){
    return GL_NO_ERROR != glGetError();
}

bool gl::
WasErrorGeneratedAndPrint(){
    GLenum error = glGetError();
    if( error != GL_NO_ERROR ){
        std::cout << "\nOpenGL error: " << gl::GetErrorString(error) << "\n";
        std::cout << " Description:  " << gl::GetErrorDescription(error) << "\n";
    }
    return GL_NO_ERROR != error;
}


void gl::
UniformFloat3(GLint location, const math::float3& v){
    glUniform3fv(location, 1, v.v);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformFloat4x4(GLint location, GLboolean transpose, const math::float4x4 &m){
    gl::UniformMatrix4fv(location, 1, transpose, &(m.m[0][0]));
}


//--------------------------------------------------
//original functions:
void gl::
Enable(GLenum cap){
    glEnable(cap);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Disable(GLenum cap){
    glDisable(cap);
    assert( !WasErrorGeneratedAndPrint() );
}


void gl::
EnableVertexAttribArray(GLuint index){
    glEnableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
DisableVertexAttribArray(GLuint index){
    glDisableVertexAttribArray(index);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
ActiveTexture(GLenum texture){
    glActiveTexture(texture);
    assert( !WasErrorGeneratedAndPrint() );
}


void gl::
DeleteTextures(GLsizei n, const GLuint *textures){
    glDeleteTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint() );
}

#pragma GCC diagnostic ignored "-Wsign-conversion"
void gl::
GenVertexArrays(GLuint n, GLuint *arrays){
    glGenVertexArrays(n, arrays);
    assert( !WasErrorGeneratedAndPrint() );
}
#pragma GCC diagnostic warning "-Wsign-conversion"

void gl::
GenBuffers(GLsizei n, GLuint *buffers){
    glGenBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
DeleteBuffers(GLsizei n, const GLuint *buffers){
    glDeleteBuffers(n, buffers);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
BindVertexArray(GLuint array){
    glBindVertexArray(array);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
BindBuffer(GLenum target, GLuint buffer){
    glBindBuffer(target, buffer);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage){
    glBufferData(target, size, data, usage);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
NamedBufferData(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage){
    glNamedBufferData(buffer, size, data, usage);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
TexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data){
    glTexImage2D(target, level, internalformat, width, height, border, format, type, data);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer){
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
VertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer){
    glVertexAttribIPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
VertexAttribLPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer){
    glVertexAttribLPointer(index, size, type, stride, pointer);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
DrawArrays(GLenum mode, GLint first, GLsizei count){
    glDrawArrays(mode, first, count);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices){
    glDrawElements(mode, count, type, indices);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
BindTexture(GLenum target, GLuint texture){
    glBindTexture(target, texture);
    assert( !WasErrorGeneratedAndPrint() );
}


void gl::
GenTextures(GLsizei n, GLuint *textures){
    glGenTextures(n, textures);
    assert( !WasErrorGeneratedAndPrint() );
}


void gl::
TexParameterf(GLenum target, GLenum pname, GLfloat param){
    glTexParameterf(target, pname, param);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
TexParameteri(GLenum target, GLenum pname, GLint param){
    glTexParameteri(target, pname, param);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1f(GLint location, GLfloat v0){
    glUniform1f(location, v0);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2f(GLint location, GLfloat v0, GLfloat v1){
    glUniform2f(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2){
    glUniform3f(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3){
    glUniform4f(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1i(GLint location, GLint v0){
    glUniform1i(location, v0);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2i(GLint location, GLint v0, GLint v1){
    glUniform2i(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3i(GLint location, GLint v0, GLint v1, GLint v2){
    glUniform3i(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3){
    glUniform4i(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1ui(GLint location, GLuint v0){
    glUniform1ui(location, v0);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2ui(GLint location, GLuint v0, GLuint v1){
    glUniform2ui(location, v0, v1);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2){
    glUniform3ui(location, v0, v1, v2);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3){
    glUniform4ui(location, v0, v1, v2, v3);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1fv(GLint location, GLsizei count, const GLfloat *value){
    glUniform1fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2fv(GLint location, GLsizei count, const GLfloat *value){
    glUniform2fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3fv(GLint location, GLsizei count, const GLfloat *value){
    glUniform3fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4fv(GLint location, GLsizei count, const GLfloat *value){
    glUniform4fv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1iv(GLint location, GLsizei count, const GLint *value){
    glUniform1iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2iv(GLint location, GLsizei count, const GLint *value){
    glUniform2iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3iv(GLint location, GLsizei count, const GLint *value){
    glUniform3iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4iv(GLint location, GLsizei count, const GLint *value){
    glUniform4iv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform1uiv(GLint location, GLsizei count, const GLuint *value){
    glUniform1uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform2uiv(GLint location, GLsizei count, const GLuint *value){
    glUniform2uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform3uiv(GLint location, GLsizei count, const GLuint *value){
    glUniform3uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
Uniform4uiv(GLint location, GLsizei count, const GLuint *value){
    glUniform4uiv(location, count, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix2x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix3x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix2x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix4x2fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix3x4fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix4x3fv(location, count, transpose, value);
    assert( !WasErrorGeneratedAndPrint() );
}

GLint gl::
GetUniformLocation(GLuint program, const GLchar *name){
    GLint retval = glGetUniformLocation(program, name);
    //-------
    //TODO: remove this
    bool result = false;
    result = !WasErrorGeneratedAndPrint();
    if(!result){
        assert(false);
    }
    //------
    //assert( !WasErrorGeneratedAndPrint() );
    return retval;
}










void gl::
ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length){
    glShaderSource(shader, count, string, length);
    assert( !WasErrorGeneratedAndPrint() );
}

GLuint gl::
CreateShader(GLenum shaderType){
    GLuint retval = glCreateShader(shaderType);
    assert( !WasErrorGeneratedAndPrint() );
    return retval;
}

void gl::
CompileShader(GLuint shader){
    glCompileShader(shader);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetShaderiv(GLuint shader, GLenum pname, GLint *params){
    glGetShaderiv(shader, pname, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog){
    glGetShaderInfoLog(shader, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
AttachShader(GLuint program, GLuint shader){
    glAttachShader(program, shader);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
LinkProgram(GLuint program){
    glLinkProgram(program);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetProgramiv(GLuint program, GLenum pname, GLint *params){
    glGetProgramiv(program, pname, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
ValidateProgram(GLuint program){
    glValidateProgram(program);
    bool success = !WasErrorGeneratedAndPrint();
    int program_validated;
    gl::GetProgramiv(program, GL_VALIDATE_STATUS, &program_validated);
    if( !program_validated ){
        std::cout << "OpenGL ERROR: could not validate ShaderProgram!\n";
    }
    assert( program_validated );
    assert( success );
}

void gl::
GetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog){
    glGetProgramInfoLog(program, maxLength, length, infoLog);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
UseProgram(GLuint program){
    glUseProgram(program);
    assert( !WasErrorGeneratedAndPrint() );
}



void gl::
GetUniformfv(GLuint program, GLint location, GLfloat *params)
{
    glGetUniformfv(program, location, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetUniformiv(GLuint program, GLint location, GLint *params)
{
    glGetUniformiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetUniformuiv(GLuint program, GLint location, GLuint *params)
{
    glGetUniformuiv(program, location, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetUniformdv(GLuint program, GLint location, GLdouble *params)
{
    glGetUniformdv(program, location, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetnUniformfv(GLuint program, GLint location, GLsizei bufSize, GLfloat *params)
{
    glGetnUniformfv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetnUniformiv(GLuint program, GLint location, GLsizei bufSize, GLint *params)
{
    glGetnUniformiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetnUniformuiv(GLuint program, GLint location, GLsizei bufSize, GLuint *params)
{
    glGetnUniformuiv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint() );
}

void gl::
GetnUniformdv(GLuint program, GLint location, GLsizei bufSize, GLdouble *params)
{
    glGetnUniformdv(program, location, bufSize, params);
    assert( !WasErrorGeneratedAndPrint() );
}

