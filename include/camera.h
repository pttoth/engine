#pragma once

#include "entity.h"

#include "pt/math.h"
#include "BasicPositionComponent.h"


#include <assert.h>

namespace engine{

class Camera: public Entity
{

public:
    enum Dir{
        FORWARD     = 0,
        BACKWARD    = 1,
        LEFT        = 2,
        RIGHT       = 3,
        UP          = 4,
        DOWN        = 5
    };

    Camera();
    virtual ~Camera(){}

    virtual void        OnRegister() override;
    virtual void        OnUnregister() override;

    void                UpdateData();
    // note: expects line vectors
    pt::math::float4x4  GetViewMtx() const;
    pt::math::float4x4  GetProjMtx() const;
    void                Move(const pt::math::float3& dir);
    void                MoveTarget(float x_angle, float y_angle);
    pt::math::float3    GetDir(Dir direction) const;

    float               GetAspectRatio() const;
    void                SetAspectRatio(float ratio);

protected:
    virtual void        tick(float t, float dt) override;

private:
    pt::math::float3    GetForward() const;
    pt::math::float3    GetBackward() const;
    pt::math::float3    GetRight() const;
    pt::math::float3    GetLeft() const;
    pt::math::float3    GetUp() const;
    pt::math::float3    GetDown() const;

//private variables
    BasicPositionComponent mBasicPosComponent;

    float   mAspectRatio;

    float   mFOV;
    float   mClippingNearDist;
    float   mClippingFarDist;

    //cached direction data
    pt::math::float3    mCamZ;
    pt::math::float3    mCamRight;
    pt::math::float3    mCamUp;

    pt::math::float3    mLookat;  //camera target coordinates
    pt::math::float3    mLookatRelative;
    pt::math::float3    mVecUp;     //peferred vertical direction (this is mostly {0,0,1} )


};

} //end of namespace engine

