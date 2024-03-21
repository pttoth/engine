#include "engine/StandardShaderProgram.h"

using namespace engine;

const pt::Name StandardShaderProgram::stNameWireframeMode( "WireframeMode" );
const pt::Name StandardShaderProgram::stNameWireframeColor( "WireframeColor" );
const pt::Name StandardShaderProgram::stNameT( "t" );
const pt::Name StandardShaderProgram::stNameDT( "dt" );
const pt::Name StandardShaderProgram::stNameM( "M" );
const pt::Name StandardShaderProgram::stNameV( "V" );
const pt::Name StandardShaderProgram::stNameVrot( "Vrot" );
const pt::Name StandardShaderProgram::stNamePV( "PV" );
const pt::Name StandardShaderProgram::stNamePVM( "PVM" );


StandardShaderProgram::
StandardShaderProgram( const pt::Name& name ):
    gl::ShaderProgram( name )
{
    mUniformNames.reserve(12);
    mUniformNames.push_back( stNameWireframeMode );
    mUniformNames.push_back( stNameWireframeColor );
    mUniformNames.push_back( stNameT );
    mUniformNames.push_back( stNameDT );
    mUniformNames.push_back( stNameM );
    mUniformNames.push_back( stNameV );
    mUniformNames.push_back( stNameVrot );
    mUniformNames.push_back( stNamePV );
    mUniformNames.push_back( stNamePVM );
}


StandardShaderProgram::
~StandardShaderProgram()
{}


void StandardShaderProgram::
SetUniformT( float val )
{
    mUniT = val;
    this->SetUniform( mUniT );
}


void StandardShaderProgram::
SetUniformDT( float val )
{
    mUniDT = val;
    this->SetUniform( mUniDT );
}


void StandardShaderProgram::
SetUniformModelMatrix( const math::float4x4& val )
{
    mUniM = val;
    this->SetUniform( mUniM );
}


void StandardShaderProgram::
SetUniformRotationMatrix( const math::float4x4& val )
{
    mUniVrot = val;
    this->SetUniform( mUniVrot );
}


void StandardShaderProgram::
SetUniformViewMatrix( const math::float4x4& val )
{
    mUniV = val;
    this->SetUniform( mUniV );
}


void StandardShaderProgram::
SetUniformViewProjectionMatrix( const math::float4x4& val )
{
    mUniPV = val;
    this->SetUniform( mUniPV );
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
    mUniT    = GetUniform<float>( stNameT );
    mUniDT   = GetUniform<float>( stNameDT );
    mUniM    = GetUniform<math::float4x4>( stNameM );
    mUniV    = GetUniform<math::float4x4>( stNameV );
    mUniVrot = GetUniform<math::float4x4>( stNameVrot );
    mUniPV   = GetUniform<math::float4x4>( stNamePV );
    mUniPVM  = GetUniform<math::float4x4>( stNamePVM );

    SetUniform( mUniWireframeMode, 0 );

    SetUniform( mUniT,      0.0f);
    SetUniform( mUniDT,     0.0f);
    SetUniform( mUniM,      math::float4x4::identity );
    SetUniform( mUniV,      math::float4x4::identity );
    SetUniform( mUniVrot,   math::float4x4::identity );
    SetUniform( mUniPV,     math::float4x4::identity );
    SetUniform( mUniPVM,    math::float4x4::identity );
}
