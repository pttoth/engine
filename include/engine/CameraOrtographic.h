/** -----------------------------------------------------------------------------
  * FILE:    CameraOrtographic.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"

#include "pt/math.h"
#include "PositionComponent.h"


#include <assert.h>

namespace engine{

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

    void UpdateData();

    // note: expects line vectors
    virtual const math::float4x4  GetRotationMtx() const override;
    virtual const math::float4x4  GetViewMtx() const override;
    virtual const math::float4x4  GetProjMtx() const override;

    virtual void    Move( const math::float3& dir ) override;

protected:
    virtual void    OnTick( float t, float dt ) override;

    const math::float3  GetForward() const override; //TODO: inherit these from abstract Camera class
    const math::float3  GetBackward() const override;
    const math::float3  GetRight() const override;
    const math::float3  GetLeft() const override;
    const math::float3  GetUp() const override;
    const math::float3  GetDown() const override;

    void OnSpawned() override;
    void OnDespawned() override;


private:
    void Construct();

};

} //end of namespace engine

