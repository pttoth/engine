/** -----------------------------------------------------------------------------
  * FILE:    CameraPerspective.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Camera.h"

#include <memory>

namespace engine{

class CameraPerspective;
using CameraPerspectivePtr       = std::shared_ptr< CameraPerspective >;
using ConstCameraPerspectivePtr  = std::shared_ptr< const CameraPerspective >;
using CameraPerspectiveWPtr      = std::weak_ptr< CameraPerspective >;
using ConstCameraPerspectiveWPtr = std::weak_ptr< const CameraPerspective >;
using CameraPerspectiveUPtr      = std::unique_ptr< CameraPerspective >;
using ConstCameraPerspectiveUPtr = std::unique_ptr< const CameraPerspective >;


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
    void OnTick(float t, float dt) override;

    void OnSpawned() override;
    void OnDespawned() override;

private:
    void Construct();
    void UpdateData();

};

} // end of namespace 'engine'
