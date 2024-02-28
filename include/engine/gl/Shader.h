#pragma once

#include "engine/gl/Def.h"
#include "pt/macros.h"

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Shader )

using ConstStdSharedPtr = std::shared_ptr<const std::string>;

class Shader
{
public:
    //Shader();
    Shader( const std::string& name, gl::ShaderType type, const ConstStdSharedPtr code );
    virtual ~Shader();
    Shader( const Shader& other ) = delete;
    Shader& operator=( const Shader& other ) = delete;
    Shader( Shader&& source ) = default;
    Shader& operator=( Shader&& source );

    bool operator==( const Shader& other ) = delete;

    bool                Compile();
    void                FreeVRAM();
    GLuint              GetHandle() const;
    const std::string&  GetName() const;
    gl::ShaderType      GetShaderType() const;
    ConstStdSharedPtr   GetSourceCode() const;
    bool                IsCompiled() const;

protected:
private:
    std::string          mName;
    const gl::ShaderType mType = gl::ShaderType::NO_SHADER_TYPE;
    GLuint               mHandle = 0;
    ConstStdSharedPtr    mSourceCode;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
