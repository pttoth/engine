/** -----------------------------------------------------------------------------
  * FILE:    Camera.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Actor.h"

#include "PositionComponent.h"
#include "pt/math.h"
#include "pt/macros.h"

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
    bool operator==( const Camera& other ) const;

    virtual void RotateXZ( float x_angle, float z_angle ) = 0;

    //TODO: verify, which is better
    // note: expects line vectors
    virtual math::float4x4  GetRotationMtx() const = 0;
    virtual math::float4x4  GetViewMtx() const = 0;
    virtual math::float4x4  GetProjMtx() const = 0;
    virtual math::float3    GetDir( Dir direction ) const;

    virtual void    Move( const math::float3& dir ) = 0;
    virtual float   GetAspectRatio() const;
    virtual void    SetAspectRatio( float ratio );
    virtual float   GetZoom() const;
    virtual void    SetZoom( float zoom );

    virtual void    SetNearClippingDistance( float val ) = 0;
    virtual void    SetFarClippingDistance( float val ) = 0;

protected:
    virtual       void          Move_NoLock( const math::float3& dir );
    virtual const math::float3  GetForward() const;
    virtual const math::float3  GetBackward() const;
    virtual const math::float3  GetRight() const;
    virtual const math::float3  GetLeft() const;
    virtual const math::float3  GetUp() const;
    virtual const math::float3  GetDown() const;

private:
    void    InitMembers();

    float   mAspectRatio;
    float   mZoom;

};

} //end of namespace 'engine'
