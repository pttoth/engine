/** -----------------------------------------------------------------------------
  * FILE:    CameraPerspective.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Camera.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( CameraPerspective )

class CameraPerspective: public Camera
{
public:
    CameraPerspective( const std::string& name );
    CameraPerspective( const CameraPerspective& other ) = delete;
    CameraPerspective( CameraPerspective&& source ) = delete;
    virtual ~CameraPerspective(){}
    CameraPerspective& operator=( const CameraPerspective& other ) = delete;
    CameraPerspective& operator=( CameraPerspective&& source ) = delete;
    bool operator==( const CameraPerspective& other ) const;

    const math::float4x4 GetRotationMtx() const override;
    const math::float4x4 GetViewMtx() const override;
    const math::float4x4 GetProjMtx() const override;

    void Move(const math::float3 &dir) override;

protected:
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

    void OnTick(float t, float dt) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void Construct();
    void UpdateData();


};

} // end of namespace 'engine'
