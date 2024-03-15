#include "engine/DefaultShaderProgram.h"

using namespace engine;

const pt::Name DefaultShaderProgram::stNameT( "t" );
const pt::Name DefaultShaderProgram::stNameDT( "dt" );
const pt::Name DefaultShaderProgram::stNameM( "M" );
const pt::Name DefaultShaderProgram::stNameV( "V" );
const pt::Name DefaultShaderProgram::stNameVrot( "Vrot" );
const pt::Name DefaultShaderProgram::stNamePV( "PV" );
const pt::Name DefaultShaderProgram::stNamePVM( "PVM" );


DefaultShaderProgram::
DefaultShaderProgram( const pt::Name& name ):
    gl::ShaderProgram( name )
{
    mUniformNames.reserve(8);
    mUniformNames.push_back( stNameT );
    mUniformNames.push_back( stNameDT );
    mUniformNames.push_back( stNameM );
    mUniformNames.push_back( stNameV );
    mUniformNames.push_back( stNameVrot );
    mUniformNames.push_back( stNamePV );
    mUniformNames.push_back( stNamePVM );
}


void DefaultShaderProgram::
SetUniformModelMatrix( const math::float4x4& val )
{
    mUniM = val;
    this->SetUniform( mUniM );
}


void DefaultShaderProgram::
SetUniformRotationMatrix( const math::float4x4& val )
{
    mUniVrot = val;
    this->SetUniform( mUniVrot );
}


void DefaultShaderProgram::
SetUniformViewMatrix( const math::float4x4& val )
{
    mUniV = val;
    this->SetUniform( mUniV );
}


void DefaultShaderProgram::
SetUniformViewProjectionMatrix( const math::float4x4& val )
{
    mUniPV = val;
    this->SetUniform( mUniPV );
}


void DefaultShaderProgram::
SetUniformModelViewProjectionMatrix( const math::float4x4& val )
{
    mUniPVM = val;
    this->SetUniform( mUniPVM );
}


void DefaultShaderProgram::
OnLinked()
{
    mUniT    = GetUniform<float>( stNameT );
    mUniDT   = GetUniform<float>( stNameDT );
    mUniM    = GetUniform<math::float4x4>( stNameM );
    mUniV    = GetUniform<math::float4x4>( stNameV );
    mUniVrot = GetUniform<math::float4x4>( stNameVrot );
    mUniPV   = GetUniform<math::float4x4>( stNamePV );
    mUniPVM  = GetUniform<math::float4x4>( stNamePVM );

    SetUniform( mUniT,      0.0f);
    SetUniform( mUniDT,     0.0f);
    SetUniform( mUniM,      math::float4x4::identity );
    SetUniform( mUniV,      math::float4x4::identity );
    SetUniform( mUniVrot,   math::float4x4::identity );
    SetUniform( mUniPV,     math::float4x4::identity );
    SetUniform( mUniPVM,    math::float4x4::identity );
}


DefaultShaderProgram::
~DefaultShaderProgram()
{}

