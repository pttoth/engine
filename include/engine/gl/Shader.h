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
    //Shader();
    Shader( const pt::Name& name, gl::ShaderType type, const ConstStdStringPtr code );
    virtual ~Shader();
    Shader( const Shader& other ) = delete;
    Shader& operator=( const Shader& other ) = delete;
    Shader( Shader&& source );
    Shader& operator=( Shader&& source );

    bool operator==( const Shader& other ) = delete;

    bool                Compile();
    void                FreeVRAM();
    GLuint              GetHandle() const;
    pt::Name            GetName() const;
    gl::ShaderType      GetShaderType() const;
    ConstStdStringPtr   GetSourceCode() const;
    bool                IsCompiled() const;

protected:
private:
    pt::Name             mName;
    gl::ShaderType       mType = gl::ShaderType::NO_SHADER_TYPE;
    GLuint               mHandle = 0;
    ConstStdStringPtr    mSourceCode;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
