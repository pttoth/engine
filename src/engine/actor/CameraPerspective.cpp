#include "engine/actor/CameraPerspective.h"

#include "engine/Utility.h"
#include "pt/alias.h"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;
using namespace math;

const float gErrorMargin = 0.0001f;

CameraPerspective::
CameraPerspective( const std::string& name ):
    Camera( name )
{
    Construct_NoLock();
}


CameraPerspective::
~CameraPerspective()
{}


void CameraPerspective::
RotateCamera( float pitch, float yaw )
{
    auto lambda = [this, pitch, yaw]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );

        vec4 preferredUp    = GetPreferredUp_NoLock();
        vec4 right          = GetRight_NoLock();
        auto root           = this->GetRootComponent_NoLock();

        mat4 rotX = math::float4x4::rotation( right.XYZ(), pitch );
        mat4 rotZ = math::float4x4::rotation( preferredUp.XYZ(), yaw );
        mat4 Mrot = root->GetRotationMtx();
        Mrot = rotZ * rotX * Mrot;
        root->SetRotation( Mrot );

        //TODO: delet dis
/*
        vec3 forward = GetLookatRelative();
        vec3 preferredUp = GetPreferredUp();
        target  = math::float4( forward, 1 );
        vec3 right = forward.cross( preferredUp );

        rotX = math::float4x4::rotation( right, pitch_angle );
        rotZ = math::float4x4::rotation( preferredUp, yaw_angle );
*/
        //target = rotZ * rotX * target;
        //SetLookatRelative( Vecf3FromVecf4( target ) );
        UpdateData_NoLock();

    };
    this->PostMessage( lambda );
}


void CameraPerspective::
LookAt( const float3& target )
{
    auto lambda = [this, target]() -> void
    {
        const float margin = PT_MATH_ERROR_MARGIN;

        //TODO: lock here, when cached data logic is available for Actor getters
        vec3 cam_pos = this->GetWorldPosition();

        if( vec3(target-cam_pos).length() < margin ){
            return;
        }

        // lock 'this'
        pt::MutexLockGuard lock( mMutActorData );
        auto root = this->GetRootComponent_NoLock();

        vec3 t_rel = (target - cam_pos).normalize();
        vec3 t_rel_XY = vec3( t_rel.x, t_rel.y, 0.0f ).normalize();

        const vec3 X = vec3::xUnit;
        float yawDeg   = RadToDeg( CalcAngle( X, t_rel_XY ) );
        float pitchDeg = RadToDeg( CalcAngle( t_rel_XY, t_rel ) );

        FRotator rotator = FRotator( pitchDeg, yawDeg, 0.0f );
        PT_LOG_DEBUG( ToString( rotator ) );
        PT_LOG_DEBUG( ToString( rotator.GetTransform() ) );

        root->SetRotation( rotator.GetTransform() );
    };
    this->PostMessage( lambda );
}


math::float4x4 CameraPerspective::
GetLookAtMtx() const
{
    pt::MutexLockGuard lock( mMutActorData );

    //const vec4 right   = GetRight();
    //const vec4 up      = GetUp();
    const vec4 dir     = GetForward(); // TODO: this has to be based on world transform

    return CalcLookAtMtx( dir, GetPreferredUp_NoLock() );

    //TODO: delet dis
    //return this->GetRootComponent_NoLock()->GetRotationMtx();

    //TODO: delet dis
/*
    mat4 lookAt = mat4::identity;
    const vec4 right   = GetRight();
    const vec4 up      = GetUp();
    const vec4 dir     = GetLookatRelative().normalize();

    // right-handed system
    //X: right  (screen horizontal)     (thumb)
    //Y: up     (screen vertical)       (pointing finger)
    //Z: -dir   (points towards viewer) (middle finger)
    lookAt.m[0][0] = right.v[0];    lookAt.m[0][1] = right.v[1];    lookAt.m[0][2] = right.v[2];
    lookAt.m[1][0] = up.v[0];       lookAt.m[1][1] = up.v[1];       lookAt.m[1][2] = up.v[2];
    lookAt.m[2][0] = -dir.v[0];     lookAt.m[2][1] = -dir.v[1];     lookAt.m[2][2] = -dir.v[2];
    lookAt.m[3][3] = 1.0f;

    return lookAt;
*/
}


math::float4x4 CameraPerspective::
GetViewMtx() const
{
    const float3 pos = this->GetRootComponent_NoLock()->GetPosition();
    float4x4  translation = float4x4::identity;

    translation.m[0][3] = -pos.x;
    translation.m[1][3] = -pos.y;
    translation.m[2][3] = -pos.z;

    return GetLookAtMtx() * translation;
}


math::float4x4 CameraPerspective::
GetProjMtx() const
{
    math::float4x4 proj;

    float FoV   = GetFOVRad_NoLock();
    float aspect = GetAspectRatio_NoLock();
    float nearZ = GetNearClippingDistance_NoLock();
    float farZ  = GetFarClippingDistance_NoLock();

    proj.m[0][0] = 1/ (tanf(FoV / 2) * aspect);
    proj.m[1][1] = 1/  tanf(FoV / 2);

    proj.m[2][2] = -1*(nearZ+farZ) / (nearZ - farZ);
    proj.m[2][3] = -2* farZ*nearZ  / (nearZ - farZ);

    proj.m[3][2] = -1.0f;
    proj.m[3][3] = 0;

    return proj;
}


void CameraPerspective::
Move( const math::float3& dir )
{
    auto lambda = [this, dir]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        Move_NoLock( dir );
        UpdateData_NoLock();
    };
    this->PostMessage( lambda );
}



bool CameraPerspective::
OnCreateRenderContext()
{
    return true;
}


void CameraPerspective::
OnDestroyRenderContext()
{}


void CameraPerspective::
OnTick( float t, float dt )
{}


void CameraPerspective::
OnSpawned()
{}


void CameraPerspective::
OnDespawned()
{}


void CameraPerspective::
Construct_NoLock()
{}


//TODO: delet dis
void CameraPerspective::
UpdateData_NoLock()
{
    //assert( false );

    // TODO: delet dis
/*
    vec3    forward = GetForward();
    vec3    right   = forward.cross( GetPreferredUp() );
    float   len     = right.length();

    if( len < gErrorMargin ){
        PT_LOG_ERR( "Gimbal lock occured, resetting camera!" );
        // set camera in the X axis direction
        SetDirections_NoLock( vec3::yUnit * -1, vec3::xUnit, vec3::zUnit );
        SetPreferredUp( vec3::zUnit );
        SetLookatRelative( vec3::xUnit );
    }else{
        SetDirections_NoLock( right.normalize(),
                              forward,
                              right.cross( forward ).normalize() );
    }
*/
}
