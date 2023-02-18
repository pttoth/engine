/** -----------------------------------------------------------------------------
  * FILE:    ShaderProgram.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Represents an OpenGL shaderprogram instance as a class.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "GlWrapper.h"

#include <vector>

namespace engine{

class ShaderProgram
{
public:

    ShaderProgram( const std::string& vertex_source,
                   const std::string& geometry_source,
                   const std::string& fragment_source );
    ShaderProgram( const std::vector< std::string >& vertex_sources,
                   const std::vector< std::string >& geometry_sources,
                   const std::vector< std::string >& fragment_sources );

    virtual ~ShaderProgram();

    //move allowed
    ShaderProgram(ShaderProgram&& source)               = default;
    ShaderProgram& operator=(ShaderProgram&& source)    = default;

    //copy forbidden
    ShaderProgram(const ShaderProgram& other)           = delete;
    ShaderProgram operator=(const ShaderProgram& other) = delete;
    bool operator==(const ShaderProgram& rhs) const     = delete;

    void CreateContext();
    void Use();

    bool IsContextCreated() const;
    bool IsInUse() const;

    void GetProgramIV(GLuint program, GLenum pname, GLint *params);


private:
    const std::vector<std::string>& GetShaders( GLenum shadertype );
    void CompileShadersOfType( GLenum shadertype, char const* shadertype_as_cstring );

    void Compile();
    void Link();


    bool    mInitialized        = false;
    bool    mInitializeFailed   = false;

    const std::vector< std::string > mVertSources;
    const std::vector< std::string > mGeomSources;
    const std::vector< std::string > mFragSources;

    std::vector< GLuint > mVertHandles;
    std::vector< GLuint > mGeomHandles;
    std::vector< GLuint > mFragHandles;

    GLuint              mHandleProgram = 0;

    mutable std::mutex mMutex;

};

}
