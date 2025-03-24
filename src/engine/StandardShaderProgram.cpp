#include "engine/StandardShaderProgram.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"
#include "engine/service/SystemControl.h"

using namespace engine;
using namespace engine::gl;


StandardShaderProgram::
StandardShaderProgram()
{}


StandardShaderProgram::
StandardShaderProgram( ShaderProgramPtr shader_program ):
    shaderprog( shader_program )
{
    SetupUniformLinks();
}


StandardShaderProgram::
~StandardShaderProgram()
{}


StandardShaderProgram::
StandardShaderProgram( const StandardShaderProgram& other ):
    shaderprog( other.shaderprog )
{
    SetupUniformLinks();
    GetUniformValuesFromShader();
}


StandardShaderProgram::
StandardShaderProgram( StandardShaderProgram&& source ):
    uniWireframeMode(   std::move( source.uniWireframeMode ) ),
    uniWireframeColor(  std::move( source.uniWireframeColor ) ),
    uniM(       std::move( source.uniM ) ),
    uniMrot(    std::move( source.uniMrot ) ),
    uniPVM(     std::move( source.uniPVM ) ),
    shaderprog( std::move( source.shaderprog ) )
{}



StandardShaderProgram& StandardShaderProgram::
operator=( const StandardShaderProgram& other )
{
    shaderprog = other.shaderprog;

    SetupUniformLinks();
    GetUniformValuesFromShader();

    return *this;
}


void StandardShaderProgram::
GetUniformValuesFromShader()
{
    if( nullptr == shaderprog ){
        return;
    }

    PT_WARN_UNIMPLEMENTED_FUNCTION
}


StandardShaderProgram& StandardShaderProgram::
operator=( StandardShaderProgram&& source )
{
    uniWireframeMode    = std::move( source.uniWireframeMode );
    uniWireframeColor   = std::move( source.uniWireframeColor );
    uniM        = std::move( source.uniM );
    uniMrot     = std::move( source.uniMrot );
    uniPVM      = std::move( source.uniPVM );
    shaderprog  = std::move( source.shaderprog );

    return *this;
}


void StandardShaderProgram::
InitializeUniformValues()
{
    if( nullptr == shaderprog ){
        PT_LOG_ERR( "StandardShaderProgram::Initialize(): 'nullptr' as 'shaderprog'!" );
        return;
    }

    shaderprog->SetUniform( uniWireframeMode, 0 );
    shaderprog->SetUniform( uniWireframeColor, math::vec3::zero );
    shaderprog->SetUniform( uniM,      math::float4x4::identity );
    shaderprog->SetUniform( uniMrot,   math::float4x4::identity );
    shaderprog->SetUniform( uniPVM,    math::float4x4::identity );
}


void StandardShaderProgram::
SetupUniformLinks()
{
    assert( nullptr != shaderprog );

    uniWireframeMode    = shaderprog->GetUniform<int>( "WireframeMode" );
    uniWireframeColor   = shaderprog->GetUniform<math::vec3>( "WireframeColor" );
    uniM                = shaderprog->GetUniform<math::float4x4>( "M" );
    uniMrot             = shaderprog->GetUniform<math::float4x4>( "Mrot" );
    uniPVM              = shaderprog->GetUniform<math::float4x4>( "PVM" );
}


void StandardShaderProgram::
LinkUniformBlockFrameInfo()
{
    const char*     ubName = "FrameInfo";
    const uint32_t  ubBindingIndex = Services::GetRenderer()->GetUniformBlockBindingFrameInfo();

    auto sc = Services::GetSystemControl();

    // case: Binding Point index is invalid
    size_t numBindingPoints = sc->GetMaximumUniformBlockBindingPoints();
    if( numBindingPoints <= ubBindingIndex ){
        PT_LOG_ERR( "Tried to bind uniform block to invalid binding point index (idx: " << ubBindingIndex << ", max: " << numBindingPoints << ")." );
        return;
    }

    GLuint idxUniBlock = gl::GetUniformBlockIndex( shaderprog->GetHandle(), ubName );
    // case: ShaderProgram is not a shader, or was never attempted to be linked (a failed link can be valid)
    if( GL_INVALID_OPERATION == gl::GetError() ){
        PT_LOG_ERR( "Tried to query Uniform Block '" << ubName << "' in ShaderProgram '" << shaderprog->GetName() << "' for which Link() was never called in the past!" );
        return;
    }
    // case: the Uniform Block does not exist in the shader
    if( GL_INVALID_INDEX == idxUniBlock ){
        PT_LOG_ERR( "Could not find Uniform Block '" << ubName << "' in ShaderProgram '" << shaderprog->GetName() << "'" );
        return;
    }

    gl::UniformBlockBinding( shaderprog->GetHandle(), idxUniBlock, ubBindingIndex );
}

