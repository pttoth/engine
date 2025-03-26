

//@TODO: add header

#pragma once

#include "engine/gl/ShaderProgram.h"
#include "engine/Def.h"
#include "pt/macros.h"


//@TODO: move StandardShaderProgram from 'engine::' to 'engine::gl::' everywhere

namespace engine{
//namespace gl{

PT_FORWARD_DECLARE_CLASS( StandardShaderProgram )

class StandardShaderProgram
{

public:
    static StandardShaderProgramPtr CreateFromDescriptorFile( const std::string& name, const std::string& path );
    static StandardShaderProgramPtr CreateFromString( const std::string& name, const std::string& data );
    static StandardShaderProgramPtr CreateFromShaderList( const std::string& name, const std::vector<gl::ShaderPtr>& shaders );

    virtual ~StandardShaderProgram();

    StandardShaderProgram( const StandardShaderProgram& other ) = delete;
    StandardShaderProgram( StandardShaderProgram&& source ) = delete;
    StandardShaderProgram& operator=( const StandardShaderProgram& other ) = delete;
    StandardShaderProgram& operator=( StandardShaderProgram&& source ) = delete;
    bool operator==( const StandardShaderProgram& other ) const = delete;

//----- public members -----
    gl::ShaderProgramPtr            program;
    gl::Uniform<int>                uniWireframeMode;
    gl::Uniform<math::vec3>         uniWireframeColor;
    gl::Uniform<math::float4x4>     uniM;
    gl::Uniform<math::float4x4>     uniMrot;
    gl::Uniform<math::float4x4>     uniPVM;
//--------------------------
protected:
    StandardShaderProgram();

private:
    void OnLinked();

};

//} // end of namespace 'gl'
} // end of namespace 'engine'
