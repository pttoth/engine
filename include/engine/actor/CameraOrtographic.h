/** -----------------------------------------------------------------------------
  * FILE:    CameraOrtographic.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"
#include "pt/macros.h"
#include <assert.h>

//TODO: implement

namespace engine{

PT_FORWARD_DECLARE_CLASS( CameraOrtographic )

class CameraOrtographic: public Camera
{

public:
    CameraOrtographic( const std::string& name );
    CameraOrtographic( const CameraOrtographic& other ) = delete;
    CameraOrtographic( Camera&& other ) = delete;
    virtual ~CameraOrtographic(){}
    CameraOrtographic& operator=( const CameraOrtographic& other ) = delete;
    CameraOrtographic& operator=( CameraOrtographic&& other ) = delete;
    bool operator==( const CameraOrtographic& other ) const = delete;

    void RotateCamera( float pitch, float yaw ) override;
    virtual void LookAt( const math::float3& lookat_pos ) override;

    // note: expects line vectors
    math::float4x4  GetLookAtMtx() const override;
    math::float4x4  GetViewMtx() const override;
    math::float4x4  GetProjMtx() const override;

protected:
    void OnTick( float t, float dt ) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void UpdateData();

};

} //end of namespace 'engine'

