/** -----------------------------------------------------------------------------
  * FILE:    CameraPerspective.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"
#include "engine/gl/Buffer.hpp"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( CameraPerspective )

class CameraPerspective: public Camera
{
public:
    CameraPerspective( const std::string& name );
    CameraPerspective( const CameraPerspective& other ) = delete;
    CameraPerspective( CameraPerspective&& source ) = delete;
    virtual ~CameraPerspective();
    CameraPerspective& operator=( const CameraPerspective& other ) = delete;
    CameraPerspective& operator=( CameraPerspective&& source ) = delete;
    bool operator==( const CameraPerspective& other ) const = delete;

    void RotateCamera( float pitch_angle, float yaw_angle ) override;
    void LookAt( const math::float3& lookat_pos ) override;

    math::float4x4 GetRotationMtx() const override;
    math::float4x4 GetViewMtx() const override;
    math::float4x4 GetProjMtx() const override;

    void Move( const math::float3& dir ) override;
    void SetNearClippingDistance( float val ) override;
    void SetFarClippingDistance( float val ) override;

    void SetFOV( const float fov );

protected:
    const math::float3 GetForward() const override;
    const math::float3 GetBackward() const override;
    const math::float3 GetRight() const override;
    const math::float3 GetLeft() const override;
    const math::float3 GetUp() const override;
    const math::float3 GetDown() const override;

    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

    void OnTick( float t, float dt ) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void Construct_NoLock();
    void UpdateData_NoLock();

    //cached direction data
    math::float3    mCamZ;
    math::float3    mCamRight;
    math::float3    mCamUp;

    math::float3    mLookatRelative = math::float3::xUnit;
    math::float3    mPreferredUp = math::float3::zUnit;

    float           mAspectRatio = 16.0f/9.0f;

    float           mFOV = 75.0f;
    float           mClippingNearDist = 1.0f;
    float           mClippingFarDist = 1000.0f;

};

} // end of namespace 'engine'