#pragma once

#include "engine/glhelpers.h"

namespace pt{

class ShaderProgram
{
    bool        m_initialied;
    GLuint      m_hVS, m_hGS, m_hFS;                        //vertex-geometry-fragment shader handles
    GLuint      m_hProgram;                                 //shader program handle
    std::string m_vs_source, m_gs_source, m_fs_source;      //shader source codes


public:
    ShaderProgram();
    virtual ~ShaderProgram();

    ShaderProgram(ShaderProgram&& source)               = default;
    ShaderProgram& operator=(ShaderProgram&& source)    = default;

    ShaderProgram(const ShaderProgram& other)           = delete;
    ShaderProgram operator=(const ShaderProgram& other) = delete;
    bool operator==(const ShaderProgram& rhs) const     = delete;


    virtual void setVertexShader(std::string& code);
    virtual void setGeometryShader(std::string& code);
    virtual void setFragmentShader(std::string& code);

    virtual bool compile();
    virtual bool link();
    virtual void use();


    //------------
        /*
        queried by: glGetProgramiv
        with arguments program and
        GL_VALIDATE_STATUS.
        */
    //------------
    //virtual bool validate();  //should be part of compile() or link()



    //https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetProgramInfoLog.xml

    virtual std::string getShaderInfoLog() const;
    virtual std::string getProgramInfoLog() const;

    virtual std::string getErrorMessage() const;

};

}
