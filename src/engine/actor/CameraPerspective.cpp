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
RotateCamera( const math::FRotator& rotator )
{
    auto lambda = [this, rotator]() -> void
    {
        if( 0.0f != rotator.mRoll ){
            PT_LOG_WARN( "RotateCamera received a rotator with a non-zero 'roll' component! (" << ToString( rotator ) << ")" );
        }

        pt::MutexLockGuard lock( mMutActorData );

        vec4 preferredUp    = GetPreferredUp_NoLock();
        vec4 right          = GetRight_NoLock();
        float pitch_angle   = rotator.mPitch;
        float yaw_angle     = rotator.mYaw;
        auto root           = this->GetRootComponent_NoLock();

        mat4 rotX = math::float4x4::rotation( right.XYZ(), pitch_angle );
        mat4 rotZ = math::float4x4::rotation( preferredUp.XYZ(), yaw_angle );
        mat4 Mrot = root->GetRotationMtx();
        Mrot = rotZ * rotX * Mrot;
        root->SetRotation( Mrot );

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
LookAt( const float3& lookat_pos )
{
    auto lambda = [this, lookat_pos]() -> void
    {
        // acquire outside data before locking Actor
        Actor* parent = this->GetParent(); // a brief lock on 'this'
        mat4 parentTransform = mat4::identity;
        if( nullptr != parent ){
            parent->GetWorldTransform(); // brief lock on parent
        }

        // lock 'this'
        pt::MutexLockGuard lock( mMutActorData );
        auto root = this->GetRootComponent_NoLock();

        vec4 lookat_rel  = parentTransform.invert() * vec4( lookat_pos, 1.0f ); // move 'lookat_pos' into parent coordinate space
        vec4 pos_rel     = vec4( root->GetPosition(), 1.0f );

        vec4 dir = lookat_rel - pos_rel;
        mat4 la = CalcLookAtMtx( dir, GetPreferredUp_NoLock() );
        root->SetRotation( la );    //TODO: FIX THIS (lookat mtx is not a rotation mtx!)
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
