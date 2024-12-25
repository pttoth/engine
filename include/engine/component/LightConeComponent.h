#pragma once

#include "engine/component/LightComponentBase.h"

#include "engine/gl/Uniform.hpp"

#include "engine/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightConeComponent )


class LightConeComponent: public LightComponentBase
{
public:
    using vec3 = math::vec3;
    using vec4 = math::vec4;
    using mat4 = math::mat4;

    LightConeComponent( const std::string& name );
    virtual ~LightConeComponent();

    LightConeComponent( const LightConeComponent& other ) = delete;
    LightConeComponent( LightConeComponent&& source ) = delete;
    LightConeComponent& operator=( const LightConeComponent& other ) = delete;
    LightConeComponent& operator=( LightConeComponent&& source ) = delete;
    bool operator==( const LightConeComponent& other ) const = delete;

    void LoadParametersToCurrentShader() override;

    void EnableLight( bool val ) override;
    bool IsLightEnabled() const override;

    void SetColor( math::vec3 color ) override;
    void SetIntensity( float intensity ) override;
    void SetRadius( float radius );
    void SetAngle( float angle );

    math::vec3  GetColor() const override;
    float       GetIntensity() const override;
    float       GetRadius() const;
    float       GetAngle() const;

protected:
    void OnRender_GL3_3( float t, float dt ) override;
    bool OnCreateRenderContext_GL3_3() override;
    bool OnDestroyRenderContext_GL3_3() override;

    void OnTick( float t, float dt ) override;

private:
    vec3    mColor      = vec3::white;
    int32_t mLightSlot  = -1;

    float   mIntensity  = 5.0f;
    float   mAngle      = 60.0f; // angle in degrees!
    float   mRadius     = 1000.0f;

    bool    mEnabled    = true;


    gl::Uniform<mat4>       mUniTransform;
    gl::Uniform<vec4>       mUniColor;
    gl::Uniform<float>      mUniIntensity;
    gl::Uniform<float>      mUniAngle;
    gl::Uniform<float>      mUniRadius;
    gl::Uniform<int>        mUniEnabled;

    bool                    mParamsDirty = true;

};

} // end of namespace 'engine'
