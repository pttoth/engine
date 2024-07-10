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

    virtual void RotateCamera( float pitch, float yaw ) = 0;
    virtual void LookAt( const math::float3& lookat_pos ) = 0;

    virtual math::float4x4  GetLookAtMtx() const = 0;   // rearranges the coordinate system axes from world space to screen space (no translation!) (not to be confused with 'GetRotationMtx()'!)
    virtual math::float4x4  GetViewMtx() const = 0;     // transforms from world space to screen space
    virtual math::float4x4  GetProjMtx() const = 0;     // applies perspective projection
    virtual math::float4    GetDir( Dir direction ) const;

    virtual void Move( const math::float3& dir );

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
    //TODO: delete these
    math::float4  GetForward() const;
    math::float4  GetBackward() const;
    math::float4  GetRight() const;
    math::float4  GetLeft() const;
    math::float4  GetUp() const;
    math::float4  GetDown() const;
    //-----

    //note: these return directions in relative system (parent coordinate system), not world
    math::float4  GetForward_NoLock() const;
    math::float4  GetBackward_NoLock() const;
    math::float4  GetRight_NoLock() const;
    math::float4  GetLeft_NoLock() const;
    math::float4  GetUp_NoLock() const;
    math::float4  GetDown_NoLock() const;

    math::float3  GetPreferredUp() const;
    void          SetPreferredUp( const math::float3& vector );

    math::float4  GetPreferredUp_NoLock() const;


private:
    float   mAspectRatio        = 1.0f;
    float   mFOV                = math::DegToRad( 90.0f );
    float   mClippingNearDist   = 1.0f;
    float   mClippingFarDist    = 100000.0f;

    math::float3    mPreferredUp    = math::float3::zUnit;

};

} //end of namespace 'engine'
