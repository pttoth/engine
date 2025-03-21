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
    StandardShaderProgram( gl::ShaderProgramPtr shader_program );
    virtual ~StandardShaderProgram();

    StandardShaderProgram( const StandardShaderProgram& other ) = delete;   //@TODO: add
    StandardShaderProgram( StandardShaderProgram&& source );
    StandardShaderProgram& operator=( const StandardShaderProgram& other ) = delete;   //@TODO: add
    StandardShaderProgram& operator=( StandardShaderProgram&& source );
    bool operator==( const StandardShaderProgram& other ) const = delete;

    void Initialize();

    // Sets up the 'FrameInfo' uniform block in the shader to the renderer's defined uniform binding point
    void LinkUniformBlockFrameInfo();

    // These won't track changes if shaderprog re-links!
    gl::Uniform<int>                uniWireframeMode;
    gl::Uniform<int>                uniWireframeColor;
    gl::Uniform<math::float4x4>     uniM;
    gl::Uniform<math::float4x4>     uniMrot;
    gl::Uniform<math::float4x4>     uniPVM;

    gl::ShaderProgramPtr            shaderprog;
};

//} // end of namespace 'gl'
} // end of namespace 'engine'
