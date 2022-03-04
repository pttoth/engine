#pragma once

#include "Camera.h"

#include "pt/math.h"
#include "BasicPositionComponent.h"


#include <assert.h>

namespace engine{

class Camera2D: public Camera
{

public:
    Camera2D();
    virtual ~Camera2D(){}

    virtual void        OnRegister() override;
    virtual void        OnUnregister() override;

    void                UpdateData();

    // note: expects line vectors
    virtual const pt::math::float4x4  GetViewMtx() const override;
    virtual const pt::math::float4x4  GetProjMtx() const override;

    virtual void    Move(const pt::math::float3& dir) override;

protected:
    virtual void    tick(float t, float dt) override;

    virtual const pt::math::float3  GetForward() const override; //TODO: inherit these from abstract Camera class
    virtual const pt::math::float3  GetBackward() const override;
    virtual const pt::math::float3  GetRight() const override;
    virtual const pt::math::float3  GetLeft() const override;
    virtual const pt::math::float3  GetUp() const override;
    virtual const pt::math::float3  GetDown() const override;

private:


};

} //end of namespace engine

