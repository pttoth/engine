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

// TODO: make threadsafe (use Actor message queue)
// OPTIMIZE: matrix calculations
// OPTIMIZE: clean/dirty private mutable matrices

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

    void RotateCamera( const math::FRotator& rotator ) override;
    void LookAt( const math::float3& target ) override;

    math::float4x4 GetLookAtMtx() const override;
    math::float4x4 GetViewMtx() const override;
    math::float4x4 GetProjMtx() const override;

    void Move( const math::float3& dir ) override;

protected:
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

    void OnTick( float t, float dt ) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void Construct_NoLock();
    void UpdateData_NoLock();

};

} // end of namespace 'engine'
