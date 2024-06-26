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

    virtual void    Move( const math::float3& dir );

    virtual float   GetAspectRatio() const;
    virtual void    SetAspectRatio( float ratio );
    virtual float   GetFOVDeg() const;
    virtual void    SetFOVDeg( float fov );
    virtual float   GetFOVRad() const;
    virtual void    SetFOVRad( float fov );

    virtual void    SetNearClippingDistance( float val ) = 0;
    virtual void    SetFarClippingDistance( float val ) = 0;

protected:
    virtual void    Move_NoLock( const math::float3& dir );

            float   GetAspectRatio_NoLock() const;
            void    SetAspectRatio_NoLock( float ratio );
            float   GetFOVRad_NoLock() const;
            void    SetFOVRad_NoLock( float fov );

//TODO: delet dis

    virtual const math::float3  GetForward() const;
    virtual const math::float3  GetBackward() const;
    virtual const math::float3  GetRight() const;
    virtual const math::float3  GetLeft() const;
    virtual const math::float3  GetUp() const;
    virtual const math::float3  GetDown() const;


private:
    float   mAspectRatio    = 1.0f;
    float   mFOV            = math::DegToRad( 90.0f );

};

} //end of namespace 'engine'
