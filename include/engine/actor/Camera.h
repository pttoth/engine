/** -----------------------------------------------------------------------------
  * FILE:    Camera.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Actor.h"

#include "engine/component/PositionComponent.h"
#include "pt/math.h"
#include "pt/macros.h"

// TODO: make threadsafe (use Actor message queue)

// OPTIMIZE: clean/dirty cached private mutable matrices (eg. 'GetDir()' using 'GetRotationMtx()' )

namespace engine{

PT_FORWARD_DECLARE_CLASS( Camera )

class Camera: public Actor
{

public:
    enum Dir{
        FORWARD     = 0,
        BACKWARD    = 1,
        LEFT        = 2,
        RIGHT       = 3,
        UP          = 4,
        DOWN        = 5
    };

    Camera( const std::string& name );
    Camera( const Camera& other ) = delete;
    Camera( Camera&& other ) = delete;
    virtual ~Camera(){}
    Camera& operator=( const Camera& other ) = delete;
    Camera& operator=( Camera&& other ) = delete;
    bool    operator==( const Camera& other ) const = delete;

    virtual void RotateCamera( float x_angle, float z_angle ) = 0;
    virtual void LookAt( const math::float3& lookat_pos ) = 0;

    //TODO: verify, which is better
    // note: expects line vectors
    virtual math::float4x4  GetLookAtMtx() const = 0;
    virtual math::float4x4  GetViewMtx() const = 0;
    virtual math::float4x4  GetProjMtx() const = 0;
    virtual math::float3    GetDir( Dir direction ) const;

    void    Move( const math::float3& dir );

    float   GetAspectRatio() const;
    void    SetAspectRatio( float ratio );
    float   GetFOVDeg() const;
    void    SetFOVDeg( float fov );
    float   GetFOVRad() const;
    void    SetFOVRad( float fov );

    void    SetNearClippingDistance( float val );
    float   GetNearClippingDistance() const;

    void    SetFarClippingDistance( float val );
    float   GetFarClippingDistance() const;

protected:
    void    Move_NoLock( const math::float3& dir );

    float   GetAspectRatio_NoLock() const;
    void    SetAspectRatio_NoLock( float ratio );
    float   GetFOVRad_NoLock() const;
    void    SetFOVRad_NoLock( float fov );

    float   GetNearClippingDistance_NoLock() const;
    void    SetNearClippingDistance_NoLock( float distance );
    float   GetFarClippingDistance_NoLock() const;
    void    SetFarClippingDistance_NoLock( float distance );

    //-----
    //TODO: add _NoLock
    math::float3  GetForward() const;
    math::float3  GetBackward() const;
    math::float3  GetRight() const;
    math::float3  GetLeft() const;
    math::float3  GetUp() const;
    math::float3  GetDown() const;

    math::float3  GetPreferredUp() const;
    void          SetPreferredUp( const math::float3& vector );

    math::float3  GetLookatRelative() const;
    void          SetLookatRelative( const math::float3& vector );
    //-----

    void          SetDirections_NoLock( const math::float3& right, const math::float3& forward, const math::float3& up );

private:
    float   mAspectRatio        = 1.0f;
    float   mFOV                = math::DegToRad( 90.0f );
    float   mClippingNearDist   = 1.0f;
    float   mClippingFarDist    = 100000.0f;

    //cached direction data
    math::float3    mCamForward = math::float3::xUnit;
    math::float3    mCamRight   = math::float3::yUnit * -1;
    math::float3    mCamUp      = math::float3::zUnit;

    math::float3    mLookatRelative = math::float3::xUnit;
    math::float3    mPreferredUp    = math::float3::zUnit;

};

} //end of namespace 'engine'
