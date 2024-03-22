#include "engine/Utility.h"

#include "pt/logging.h"

#include <sstream>
#include <assert.h>

math::float4x4 engine::
CalcMVP( const engine::Actor& actor, const engine::Camera& camera )
{
    return camera.GetProjMtx() * camera.GetViewMtx() * actor.GetWorldTransform();
}


math::float4x4 engine::
CalcMVP( const engine::WorldComponent& component, const engine::Camera& camera )
{
    return camera.GetProjMtx() * camera.GetViewMtx() * component.GetWorldTransform();
}


math::float4x4 engine::
CalcRotMtx( const math::float3& direction, const math::float3& preferredUp )
{
    PT_UNIMPLEMENTED_FUNCTION
    return math::float4x4();
}


math::float4x4 engine::
CalcScaleMtx(const math::float3& vec )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][0] = vec.v[0];
    mtx.m[1][1] = vec.v[1];
    mtx.m[2][2] = vec.v[2];
    return mtx;
}

math::float4x4 engine::
CalcTranslationMtx(const math::float3& vec )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][3] = vec.v[0];
    mtx.m[1][3] = vec.v[1];
    mtx.m[2][3] = vec.v[2];
    return mtx;
}
