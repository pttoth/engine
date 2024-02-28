#pragma once

#include "engine/Def.h"
#include "engine/gl/Shader.h"
#include "pt/macros.h"
#include <vector>

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( ShaderProgram )

class ShaderProgram
{
public:
    //ShaderProgram();
    ShaderProgram( const std::string& name );
    virtual ~ShaderProgram();
    ShaderProgram( ShaderProgram&& source )            = default;
    ShaderProgram& operator=( ShaderProgram&& source ) = default;

    ShaderProgram( const ShaderProgram& other )             = delete;
    ShaderProgram& operator=( const ShaderProgram& other )  = delete;
    bool operator==( const ShaderProgram& other ) const     = delete;

    void AddShader( ShaderPtr shader );
    void ClearShaders();
    void FreeVRAM();
    bool Link();
    void Use();

protected:
private:
    bool                    mLinked = false;
    std::string             mName;
    std::vector<ShaderPtr>  mShaders;
    GLuint                  mHandle = 0;
};

} // end of namespace 'gl'
} // end of namespace 'engine'
