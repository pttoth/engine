#pragma once

#include "engine/gl/ShaderProgram.h"
#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( DefaultShaderProgram )

class DefaultShaderProgram: public engine::gl::ShaderProgram
{
public:
    //DefaultShaderProgram();
    DefaultShaderProgram( const pt::Name& name );
    virtual ~DefaultShaderProgram();

    DefaultShaderProgram( const DefaultShaderProgram& other ) = delete;
    DefaultShaderProgram( DefaultShaderProgram&& source ) = delete;
    DefaultShaderProgram& operator=( const DefaultShaderProgram& other ) = delete;
    DefaultShaderProgram& operator=( DefaultShaderProgram&& source ) = delete;
    bool operator==( const DefaultShaderProgram& other ) const = delete;

    void SetUniformModelMatrix( const math::float4x4& val );
    void SetUniformRotationMatrix( const math::float4x4& val );
    void SetUniformViewMatrix( const math::float4x4& val );
    void SetUniformViewProjectionMatrix( const math::float4x4& val );
    void SetUniformModelViewProjectionMatrix( const math::float4x4& val );

protected:
    void OnLinked() override;

private:
    static const pt::Name stNameT;
    static const pt::Name stNameDT;
    static const pt::Name stNameM;
    static const pt::Name stNameV;
    static const pt::Name stNameVrot;
    static const pt::Name stNamePV;
    static const pt::Name stNamePVM;

    gl::Uniform<float>  mUniT;
    gl::Uniform<float>  mUniDT;
    gl::Uniform<math::float4x4>  mUniM;
    gl::Uniform<math::float4x4>  mUniV;
    gl::Uniform<math::float4x4>  mUniVrot;
    gl::Uniform<math::float4x4>  mUniPV;
    gl::Uniform<math::float4x4>  mUniPVM;

};

} // end of namespace 'engine'
