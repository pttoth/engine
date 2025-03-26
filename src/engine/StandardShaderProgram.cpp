#include "engine/StandardShaderProgram.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"
#include "engine/service/SystemControl.h"

using namespace engine;
using namespace engine::gl;


StandardShaderProgramPtr StandardShaderProgram::
CreateFromDescriptorFile( const std::string& name , const std::string& path )
{
    StandardShaderProgramPtr instance = StandardShaderProgramPtr( new StandardShaderProgram() );
    instance->program = gl::ShaderProgram::CreateFromDescriptorFile( name, path );
    instance->program->evOnLinked.addCallback( [instance]{
        instance->OnLinked();
    } );

    return instance;
}


StandardShaderProgramPtr StandardShaderProgram::
CreateFromString( const std::string& name, const std::string& data )
{
    StandardShaderProgramPtr instance = StandardShaderProgramPtr( new StandardShaderProgram() );
    instance->program = gl::ShaderProgram::CreateFromString( name, data );
    instance->program->evOnLinked.addCallback( [instance]{
        instance->OnLinked();
    } );

    return instance;
}


StandardShaderProgramPtr StandardShaderProgram::
CreateFromShaderList( const std::string& name, const std::vector<gl::ShaderPtr>& shaders )
{
    StandardShaderProgramPtr instance = StandardShaderProgramPtr( new StandardShaderProgram() );
    instance->program = gl::ShaderProgram::CreateFromShaderList( name, shaders );
    instance->program->evOnLinked.addCallback( [instance]{
        instance->OnLinked();
    } );

    return instance;
}


StandardShaderProgram::
~StandardShaderProgram()
{}


StandardShaderProgram::
StandardShaderProgram()
{}


void StandardShaderProgram::
OnLinked()
{
    uniWireframeMode    = program->GetUniform<int>( "WireframeMode" );
    uniWireframeColor   = program->GetUniform<math::vec3>( "WireframeColor" );
    uniM                = program->GetUniform<math::float4x4>( "M" );
    uniMrot             = program->GetUniform<math::float4x4>( "Mrot" );
    uniPVM              = program->GetUniform<math::float4x4>( "PVM" );

    program->SetUniform( uniWireframeMode, 0 );
    program->SetUniform( uniWireframeColor, math::vec3::zero );
    program->SetUniform( uniM,      math::float4x4::identity );
    program->SetUniform( uniMrot,   math::float4x4::identity );
    program->SetUniform( uniPVM,    math::float4x4::identity );

    const char*     ubName = "FrameInfo";

    // Index of the Uniform Binding Point, where the FrameInfo buffer is bound
    const uint32_t  ubBindingIndex = Services::GetRenderer()->GetUniformBlockBindingFrameInfo();

    auto sc = Services::GetSystemControl();

    // case: Binding Point index is invalid
    size_t numBindingPoints = sc->GetMaximumUniformBlockBindingPoints();
    if( numBindingPoints <= ubBindingIndex ){
        PT_LOG_ERR( "Tried to bind uniform block to invalid binding point index (idx: " << ubBindingIndex << ", max: " << numBindingPoints << ")." );
        return;
    }

    // Index of the FrameInfo uniform block inside the shader program
    GLuint idxUniBlock = gl::GetUniformBlockIndex( program->GetHandle(), ubName );
    // case: ShaderProgram is not a shader, or was never attempted to be linked (a failed link can be valid)
    if( GL_INVALID_OPERATION == gl::GetError() ){
        PT_LOG_ERR( "Tried to query Uniform Block '" << ubName << "' in ShaderProgram '" << program->GetName() << "' for which Link() was never called in the past!" );
        return;
    }
    // case: the Uniform Block does not exist in the shader
    if( GL_INVALID_INDEX == idxUniBlock ){
        PT_LOG_ERR( "Could not find Uniform Block '" << ubName << "' in ShaderProgram '" << program->GetName() << "'" );
        return;
    }

    gl::UniformBlockBinding( program->GetHandle(), idxUniBlock, ubBindingIndex );
}
