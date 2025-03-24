

//@TODO: add header

#pragma once

#include "engine/gl/ShaderProgram.h"
#include "engine/Def.h"
#include "pt/macros.h"


//@TODO: move StandardShaderProgram from 'engine::' to 'engine::gl::' everywhere

namespace engine{
//namespace gl{

PT_FORWARD_DECLARE_STRUCT( StandardShaderProgram )

struct StandardShaderProgram
{
    StandardShaderProgram();
    StandardShaderProgram( gl::ShaderProgramPtr shader_program );
    virtual ~StandardShaderProgram();

    StandardShaderProgram( const StandardShaderProgram& other );
    StandardShaderProgram( StandardShaderProgram&& source );
    StandardShaderProgram& operator=( const StandardShaderProgram& other );
    StandardShaderProgram& operator=( StandardShaderProgram&& source );
    bool operator==( const StandardShaderProgram& other ) const = delete;

    void GetUniformValuesFromShader();
    void InitializeUniformValues();
    void SetupUniformLinks();

    // Sets up the 'FrameInfo' uniform block in the shader to the renderer's defined uniform binding point
    void LinkUniformBlockFrameInfo();

    // These won't track changes if shaderprog re-links!
    gl::Uniform<int>                uniWireframeMode;
    gl::Uniform<math::vec3>         uniWireframeColor;
    gl::Uniform<math::float4x4>     uniM;
    gl::Uniform<math::float4x4>     uniMrot;
    gl::Uniform<math::float4x4>     uniPVM;

    gl::ShaderProgramPtr            shaderprog;
};

//} // end of namespace 'gl'
} // end of namespace 'engine'
