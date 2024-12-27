#pragma once

#include "engine/gl/Def.h"
#include "pt/alias.h"
#include "pt/macros.h"
#include "pt/name.h"

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Shader )



class Shader
{
public:
    virtual ~Shader();

    static ShaderPtr    CreateFromFile( const std::string& name, gl::ShaderType type, const std::string& path );

    bool                Compile();
    void                FreeVRAM();
    GLuint              GetHandle() const;
    std::string         GetName() const;
    gl::ShaderType      GetShaderType() const;
    ConstStdStringPtr   GetSourceCode() const;
    bool                IsCompiled() const;

protected:
    Shader();
    Shader( const std::string& name, gl::ShaderType type, const ConstStdStringPtr code );
    Shader( const Shader& other ) = delete;
    Shader( Shader&& source ) = delete;
    Shader& operator=( const Shader& other ) = delete;
    Shader& operator=( Shader&& source ) = delete;
    bool operator==( const Shader& other ) = delete;

private:
    bool                mFailedCompilation = false;
    std::string          mName;
    gl::ShaderType       mType = gl::ShaderType::NO_SHADER_TYPE;
    std::string          mPath;
    GLuint               mHandle = 0;
    ConstStdStringPtr    mSourceCode;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
