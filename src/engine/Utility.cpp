#include "engine/Utility.h"

#include "pt/logging.h"

#include <sstream>
#include <assert.h>

using namespace math;

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
CalcLookAtMtx( const math::float3& target, const math::float3& preferredUp )
{
    const vec3 dir   = target.normalize();
    const vec3 right = dir.cross( preferredUp ).normalize();
    const vec3 up    = right.cross( dir );

    mat4 lookAt = mat4::identity;
    // right-handed system
    //X: right  (screen horizontal)     (thumb)
    //Y: up     (screen vertical)       (pointing finger)
    //Z: -dir   (points towards viewer) (middle finger)
    lookAt.m[0][0] = right.v[0];    lookAt.m[0][1] = right.v[1];    lookAt.m[0][2] = right.v[2];
    lookAt.m[1][0] = up.v[0];       lookAt.m[1][1] = up.v[1];       lookAt.m[1][2] = up.v[2];
    lookAt.m[2][0] = -dir.v[0];     lookAt.m[2][1] = -dir.v[1];     lookAt.m[2][2] = -dir.v[2];
    lookAt.m[3][3] = 1.0f;

    return lookAt;
}


math::float4x4 engine::
CalcLookAtMtx( const math::float4& target, const math::float4& preferredUp )
{
    return CalcLookAtMtx( target.XYZ(), preferredUp.XYZ() );
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


std::string engine::
ResolveMediaFilePath( const std::string& path )
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
    if( 0 == path.length() ){
        return std::string();
    }
    return std::string( "../../media/" ) + path;
}


