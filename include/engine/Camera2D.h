/** -----------------------------------------------------------------------------
  * FILE:    Camera2D.h
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

class Camera2D: public Camera
{

public:
    Camera2D(const std::string& name);
    Camera2D(const Camera2D& other);
    Camera2D(Camera&& other) = delete;
    virtual ~Camera2D(){}
    Camera2D& operator=(const Camera2D& other) = delete;
    Camera2D& operator=(Camera2D&& other) = delete;
    bool operator==(const Camera2D& other) const;

    void UpdateData();

    // note: expects line vectors
    virtual const pt::math::float4x4  GetViewMtx() const override;
    virtual const pt::math::float4x4  GetProjMtx() const override;

    virtual void    Move(const pt::math::float3& dir) override;

protected:
    virtual void    OnTick(float t, float dt) override;

    virtual const pt::math::float3  GetForward() const override; //TODO: inherit these from abstract Camera class
    virtual const pt::math::float3  GetBackward() const override;
    virtual const pt::math::float3  GetRight() const override;
    virtual const pt::math::float3  GetLeft() const override;
    virtual const pt::math::float3  GetUp() const override;
    virtual const pt::math::float3  GetDown() const override;

private:
    void Construct();


    // Actor interface
public:
    virtual void OnSpawned() override;
    virtual void OnDespawned() override;
};

} //end of namespace engine

