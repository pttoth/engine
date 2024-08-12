#pragma once

#include "engine/gl/ShaderProgram.h"
#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( StandardShaderProgram )

class StandardShaderProgram: public engine::gl::ShaderProgram
{
public:
    //DefaultShaderProgram();
    StandardShaderProgram( const pt::Name& name );
    virtual ~StandardShaderProgram();

    StandardShaderProgram( const StandardShaderProgram& other ) = delete;
    StandardShaderProgram( StandardShaderProgram&& source ) = delete;
    StandardShaderProgram& operator=( const StandardShaderProgram& other ) = delete;
    StandardShaderProgram& operator=( StandardShaderProgram&& source ) = delete;
    bool operator==( const StandardShaderProgram& other ) const = delete;

    void SetUniformT( float val );
    void SetUniformDT( float val );
    void SetUniformModelMatrix( const math::float4x4& val );
    void SetUniformRotationMatrix( const math::float4x4& val );
    void SetUniformViewMatrix( const math::float4x4& val );
    void SetUniformViewProjectionMatrix( const math::float4x4& val );
    void SetUniformModelViewProjectionMatrix( const math::float4x4& val );

protected:
    void OnLinked() override;
    void LinkUniformBlockFrameInfo(); // Binds the ShaderProgram's uniform FrameInfo block to DrawingControl's FrameInfo binding point

private:
    static const pt::Name stNameWireframeMode;
    static const pt::Name stNameWireframeColor;
    static const pt::Name stNameM;
    static const pt::Name stNameMrot;
    static const pt::Name stNamePVM;

    gl::Uniform<int>             mUniWireframeMode;
    gl::Uniform<int>             mUniWireframeColor;
    gl::Uniform<math::float4x4>  mUniM;
    gl::Uniform<math::float4x4>  mUniMrot;
    gl::Uniform<math::float4x4>  mUniPVM;

};

} // end of namespace 'engine'
