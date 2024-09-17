#pragma once

#include "engine/component/LightComponentBase.h"

#include "engine/gl/Uniform.hpp"

#include "engine/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightPointComponent )

class LightPointComponent: public LightComponentBase
{
public:
    using vec3 = math::vec3;
    using vec4 = math::vec4;
    using mat4 = math::mat4;

    LightPointComponent( const std::string& name );
    virtual ~LightPointComponent();

    LightPointComponent() = delete;
    LightPointComponent( const LightPointComponent& other ) = delete;
    LightPointComponent( LightPointComponent&& source ) = delete;
    LightPointComponent& operator=( const LightPointComponent& other ) = delete;
    LightPointComponent& operator=( LightPointComponent&& source ) = delete;
    bool operator==( const LightPointComponent& other ) const = delete;

    void LoadParametersToCurrentShader() override;

    void EnableLight( bool val ) override;
    bool IsLightEnabled() const override;

    void SetColor( math::vec3 color ) override;
    void SetIntensity( float intensity ) override;
    void SetRadius( float radius );

    math::vec3  GetColor() const override;
    float       GetIntensity() const override;
    float       GetRadius() const;

protected:
    void OnDraw( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

    void OnTick( float t, float dt ) override;

private:
    vec3    mColor      = vec3::white;
    int32_t mLightSlot  = -1;
    float   mIntensity  = 3.0f;
    float   mRadius     = 1000.0f;
    bool    mEnabled    = true;

    gl::Uniform<vec4>       mUniPos;        //@TODO: make vec4 for padding reasons (when used in uniform blocks)
    gl::Uniform<vec4>       mUniColor;
    gl::Uniform<float>      mUniIntensity;
    gl::Uniform<float>      mUniRadius;
    gl::Uniform<int>        mUniEnabled;

    bool                    mParamsDirty = true;
};

} // end of namespace 'engine'
