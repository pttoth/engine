/** -----------------------------------------------------------------------------
  * FILE:    CameraOrtographic.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"

#include <assert.h>

namespace engine{

class CameraOrtographic;
using CameraOrtographicPtr       = std::shared_ptr< CameraOrtographic >;
using ConstCameraOrtographicPtr  = std::shared_ptr< const CameraOrtographic >;
using CameraOrtographicWPtr      = std::weak_ptr< CameraOrtographic >;
using ConstCameraOrtographicWPtr = std::weak_ptr< const CameraOrtographic >;
using CameraOrtographicUPtr      = std::unique_ptr< CameraOrtographic >;
using ConstCameraOrtographicUPtr = std::unique_ptr< const CameraOrtographic >;

class CameraOrtographic: public Camera
{

public:
    CameraOrtographic( const std::string& name );
    CameraOrtographic( const CameraOrtographic& other ) = delete;
    CameraOrtographic( Camera&& other ) = delete;
    virtual ~CameraOrtographic(){}
    CameraOrtographic& operator=( const CameraOrtographic& other ) = delete;
    CameraOrtographic& operator=( CameraOrtographic&& other ) = delete;
    bool operator==( const CameraOrtographic& other ) const;

    // note: expects line vectors
    const math::float4x4  GetRotationMtx() const override;
    const math::float4x4  GetViewMtx() const override;
    const math::float4x4  GetProjMtx() const override;

    void Move( const math::float3& dir ) override;

protected:
    void OnTick( float t, float dt ) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void Construct();
    void UpdateData();

};

} //end of namespace 'engine'

