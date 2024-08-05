#pragma once

#include "engine/actor/Actor.h"

#include "engine/Def.h"
#include "pt/macros.h"

#include "pt/math.h"

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( LightCone )

class LightCone: public Actor
{
public:
    using vec3 = math::vec3;
    using vec4 = math::vec4;

    LightCone( const std::string& name );
    virtual ~LightCone();

    LightCone( const LightCone& other ) = delete;
    LightCone( LightCone&& source ) = delete;
    LightCone& operator=( const LightCone& other ) = delete;
    LightCone& operator=( LightCone&& source ) = delete;

    bool operator==( const LightCone& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    vec4    mColor      = vec4( vec3::white, 1.0f );
    int32_t mLightSlot  = -1;

    float   mIntensity  = 1.0f;
    float   mAngle      = 60.0f; // angle in degrees!
    float   mRadius     = 1000.0f;
};

} // end of namespace 'gl'
} // end of namespace 'engine'
