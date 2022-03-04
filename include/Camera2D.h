#pragma once

#include "entity.h"

#include "pt/math.h"
#include "BasicPositionComponent.h"


#include <assert.h>

namespace engine{

class Camera2D: public Entity
{

public:
    //TODO: move this into abstract Camera class
    enum Dir{
        FORWARD     = 0,
        BACKWARD    = 1,
        LEFT        = 2,
        RIGHT       = 3,
        UP          = 4,
        DOWN        = 5
    };

    Camera2D();
    virtual ~Camera2D(){}

    virtual void        OnRegister() override;
    virtual void        OnUnregister() override;

    void                UpdateData();
    // note: expects line vectors
    pt::math::float4x4  GetViewMtx() const;
    pt::math::float4x4  GetProjMtx() const;
    void                Move(const pt::math::float3& dir);
    pt::math::float3    GetDir(Dir direction) const;

    float               GetAspectRatio() const;
    void                SetAspectRatio(float ratio);

protected:
    virtual void        tick(float t, float dt) override;

private:
    const pt::math::float3 GetForward() const; //TODO: inherit these from abstract Camera class
    const pt::math::float3 GetBackward() const;
    const pt::math::float3 GetRight() const;
    const pt::math::float3 GetLeft() const;
    const pt::math::float3 GetUp() const;
    const pt::math::float3 GetDown() const;

//private variables
    BasicPositionComponent mBasicPosComponent;

    float   mAspectRatio;
    float   mZoom;


};

} //end of namespace engine

