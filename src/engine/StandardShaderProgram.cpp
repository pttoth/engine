#include "engine/StandardShaderProgram.h"

#include "engine/Services.h"
#include "engine/service/DrawingControl.h"
#include "engine/service/SystemControl.h"

using namespace engine;

const pt::Name StandardShaderProgram::stNameWireframeMode( "WireframeMode" );
const pt::Name StandardShaderProgram::stNameWireframeColor( "WireframeColor" );
const pt::Name StandardShaderProgram::stNameM( "M" );
const pt::Name StandardShaderProgram::stNamePVM( "PVM" );


StandardShaderProgram::
StandardShaderProgram( const pt::Name& name ):
    gl::ShaderProgram( name )
{
    mUniformNames.reserve(12);
    mUniformNames.push_back( stNameWireframeMode );
    mUniformNames.push_back( stNameWireframeColor );
    mUniformNames.push_back( stNameM );
    mUniformNames.push_back( stNamePVM );
}


StandardShaderProgram::
~StandardShaderProgram()
{}


void StandardShaderProgram::
SetUniformModelMatrix( const math::float4x4& val )
{
    mUniM = val;
    this->SetUniform( mUniM );
}


void StandardShaderProgram::
SetUniformModelViewProjectionMatrix( const math::float4x4& val )
{
    mUniPVM = val;
    this->SetUniform( mUniPVM );
}


void StandardShaderProgram::
OnLinked()
{
    mUniWireframeMode = GetUniform<int>( stNameWireframeMode );
    mUniM    = GetUniform<math::float4x4>( stNameM );
    mUniPVM  = GetUniform<math::float4x4>( stNamePVM );

    SetUniform( mUniWireframeMode, 0 );
    SetUniform( mUniM,      math::float4x4::identity );
    SetUniform( mUniPVM,    math::float4x4::identity );

    LinkUniformBlockFrameInfo();
}


void StandardShaderProgram::
LinkUniformBlockFrameInfo()
{
    const char*     ubName = "FrameInfo";
    const uint32_t  ubBindingIndex = Services::GetDrawingControl()->GetUniformBlockBindingFrameInfo();

    auto dc = Services::GetDrawingControl();
    auto sc = Services::GetSystemControl();

    const uint32_t index = dc->GetUniformBlockBindingFrameInfo();

    // case: Binding Point index is invalid
    size_t numBindingPoints = sc->GetMaximumUniformBlockBindingPoints();
    if( numBindingPoints <= index ){
        PT_LOG_ERR( "Tried to bind uniform block to invalid binding point index (idx: " << index << ", max: " << numBindingPoints << ")." );
        return;
    }

    GLuint idxUniBlock = gl::GetUniformBlockIndex( this->GetHandle(), ubName );
    // case: ShaderProgram is not a shader, or was never attempted to be linked (a failed link can be valid)
    if( GL_INVALID_OPERATION == gl::GetError() ){
        PT_LOG_ERR( "Tried to query Uniform Block '" << ubName << "' in ShaderProgram '" << this->GetName() << "' for which Link() was never called in the past!" );
        return;
    }
    // case: the Uniform Block does not exist in the shader
    if( GL_INVALID_INDEX == idxUniBlock ){
        PT_LOG_ERR( "Could not find Uniform Block '" << ubName << "' in ShaderProgram '" << this->GetName() << "'" );
        return;
    }

    gl::UniformBlockBinding( this->GetHandle(), idxUniBlock, ubBindingIndex );
}

