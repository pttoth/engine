#include "engine/Utility.h"

#include "pt/logging.h"

#include <sstream>
#include <assert.h>
/*
std::string engine::
ToString( const math::float3& vec )
{
    std::stringstream ss;
    ss << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return ss.str();
}

std::string engine::
ToString( const math::float4x4& mat )
{
    std::stringstream ss;
    ss << "(" << mat._00 << ", " << mat._01 << ", " << mat._02 << ", " << mat._03 << ")\n";
    ss << "(" << mat._10 << ", " << mat._11 << ", " << mat._12 << ", " << mat._13 << ")\n";
    ss << "(" << mat._20 << ", " << mat._21 << ", " << mat._22 << ", " << mat._23 << ")\n";
    ss << "(" << mat._30 << ", " << mat._31 << ", " << mat._32 << ", " << mat._33 << ")";
    return ss.str();
}
*/

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


//TODO: remove
math::float4x4 engine::
CalcRotMtx( const math::float3& direction, const math::float3& preferredUp )
{
    PT_LOG_ONCE_WARN( "'Utility::CalcRotMtx()' has to be reviewed/reimplemented!" );
    static bool firsttime = true;
    if( firsttime ){
        PT_WARN_UNIMPLEMENTED_FUNCTION // TEMP: display stacktrace of usages
        firsttime = false;
    }

    assert( 0 != direction.length() );
    if( 0 == direction.length() ){
        PT_LOG_ERR( "CalcRotMtx: null-vector supplied as 'direction!'" );
        return math::float4x4::identity;
    }
    math::float3 dir = direction.normalize();
    math::float3 right = dir.cross( preferredUp.normalize() );

    bool gimbal_lock = !(0.0f < right.length());
    if( gimbal_lock ){
        PT_LOG_ERR( "CalcRotMtx: Gimbal lock occured!" );
        PT_LOG_ERR( "dir: \n" << ToString(dir) );
        PT_LOG_ERR( "preferredUp: \n" << ToString(preferredUp) );
        assert( !gimbal_lock );
        return math::float4x4::identity;
    }
    math::float3 up = right.cross( dir );
    math::float4x4  orient = math::float4x4::identity;

    //(World)X stays    X(Screen)
    //(World)Y becomes -Z(Screen)
    //(World)Z becomes  Y(Screen)
    orient.m[0][0] = right.v[0];    orient.m[0][1] = right.v[1];    orient.m[0][2] = right.v[2];
    orient.m[1][0] = up.v[0];       orient.m[1][1] = up.v[1];       orient.m[1][2] = up.v[2];
    orient.m[2][0] = -dir.v[0];     orient.m[2][1] = -dir.v[1];     orient.m[2][2] = -dir.v[2];

    return orient;
}


math::float4x4 engine::
CalcScaleMtx( const math::float3& scale )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][0] = scale.v[0];
    mtx.m[1][1] = scale.v[1];
    mtx.m[2][2] = scale.v[2];
    return mtx;
}

math::float4x4 engine::
CalcTranslationMtx( const math::float3& vector )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][3] = vector.v[0];
    mtx.m[1][3] = vector.v[1];
    mtx.m[2][3] = vector.v[2];
    return mtx;
}
