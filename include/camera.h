#pragma once

#include "pt/math.h"

#include <assert.h>

namespace engine{

class Camera{

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
    void                UpdateData();
    // note: expects line vectors
    pt::math::float4x4  GetViewMtx() const;
    pt::math::float4x4  GetProjMtx() const;
    void                Move(const pt::math::float3& dir);
    void                MoveTarget(float x_angle, float y_angle);
    pt::math::float3    GetDir(Dir direction) const;

    float               GetAspectRatio() const;
    void                SetAspectRatio(float ratio);

private:
    pt::math::float3    GetForward() const;
    pt::math::float3    GetBackward() const;
    pt::math::float3    GetRight() const;
    pt::math::float3    GetLeft() const;
    pt::math::float3    GetUp() const;
    pt::math::float3    GetDown() const;

    //cached direction data
    pt::math::float3    mCamZ;
    pt::math::float3    mCamRight;
    pt::math::float3    mCamUp;

    pt::math::float3    mPos;     //camera position
    pt::math::float3    mLookat;  //camera target coordinates
    pt::math::float3    mLookatRelative;
    pt::math::float3    mVecUp;     //peferred vertical direction (this is mostly {0,0,1} )

    float           mAspectRatio;

    float           mFOV;
    float           mClippingNearDist;
    float           mClippingFarDist;

};

} //end of namespace engine

