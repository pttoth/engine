#include "include/shaderprogram.h"

#include <vector>

using namespace pttoth;

ShaderProgram::
ShaderProgram(){
}

ShaderProgram::
~ShaderProgram(){
}

void ShaderProgram::
setVertexShader(std::string &code){
    m_vs_source = code;
}

void ShaderProgram::
setGeometryShader(std::string &code){
    m_gs_source = code;
}

void ShaderProgram::
setFragmentShader(std::string &code){
    m_fs_source = code;
}

bool ShaderProgram::
compile(){


    GLuint      vs_obj      = gl::CreateShader(GL_VERTEX_SHADER);
    int         vs_length   = static_cast<int>(m_vs_source.length());
    const char* vs_source   = m_vs_source.data();
    gl::ShaderSource(vs_obj, 1, &(vs_source), &vs_length);

    GLuint      gs_obj      = gl::CreateShader(GL_GEOMETRY_SHADER);
    int         gs_length   = static_cast<int>(m_gs_source.length());
    const char* gs_source   = m_gs_source.data();
    gl::ShaderSource(gs_obj, 1, &(gs_source), &gs_length);

    GLuint      fs_obj      = gl::CreateShader(GL_FRAGMENT_SHADER);
    int         fs_length   = static_cast<int>(m_fs_source.length());
    const char* fs_source   = m_fs_source.data();
    gl::ShaderSource(fs_obj, 1, &(fs_source), &fs_length);

    std::vector<GLuint>         shaderobjects;
    std::vector<std::string>    shadernames;
    shaderobjects.push_back(vs_obj);    shadernames.push_back("vertex shader");
    //shaderobjects.push_back(gs_obj);    shadernames.push_back("geometry shader");
    shaderobjects.push_back(fs_obj);    shadernames.push_back("fragment shader");

    GLint success;
    for(size_t i = 0; i<shaderobjects.size(); ++i){
        GLuint      shader      = shaderobjects[i];
        std::string shadername  = shadernames[i];
        gl::CompileShader(shader);
        gl::GetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar InfoLog[1024];
            gl::GetShaderInfoLog(shader, sizeof(InfoLog), nullptr, InfoLog);
            fprintf(stderr, "\n  %s compilation failed:\n    '%s'\n", shadername.data(), InfoLog);
            return false;
        }
    }
    return true;
}

bool ShaderProgram::
link(){
    int success = false;    //todo: fix this

    gl::AttachShader(m_hProgram, m_hVS);
    //gl::AttachShader(m_hProgram, m_hGS);
    gl::AttachShader(m_hProgram, m_hFS);
    gl::LinkProgram(m_hProgram);

    gl::GetProgramiv(m_hProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar ErrorLog[1024];
        gl::GetProgramInfoLog(m_hProgram, sizeof(ErrorLog), nullptr, ErrorLog);
        fprintf(stderr, "\n  Error linking shader program: '%s'\n", ErrorLog);
    }

    gl::ValidateProgram(m_hProgram);
    success = true;

    return success;
}

void ShaderProgram::
use(){
    gl::UseProgram(m_hProgram);
}

std::string ShaderProgram::
getShaderInfoLog() const{
    return "shader info log unavailable (missing implementation)";
}

std::string ShaderProgram::
getProgramInfoLog() const{
    return "program info log unavailable (missing implementation)";
}

std::string ShaderProgram::
getErrorMessage() const{
    return "error msg unavailable (missing implementation)";
}

