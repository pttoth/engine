#include "camera.h"

using namespace engine;

pt::math::float3 Camera::
GetForward() const
{
    pt::math::float3 dir = mCamZ;
    dir[0]*= -1;
    dir[1]*= -1;
    dir[2]*= -1;
    return dir;
}


pt::math::float3 Camera::
GetBackward() const
{
    pt::math::float3 dir = mCamZ;
    return dir;
}


pt::math::float3 Camera::
GetRight() const
{
    pt::math::float3 dir = mCamRight;
    return dir;
}


pt::math::float3 Camera::
GetLeft() const
{
    pt::math::float3 dir = mCamRight;
    dir[0]*= -1;
    dir[1]*= -1;
    dir[2]*= -1;
    return dir;
}


pt::math::float3 Camera::
GetUp() const
{
    pt::math::float3 dir = mCamUp;
    return dir;
}


pt::math::float3 Camera::
GetDown() const
{
    pt::math::float3 dir = mCamUp;
    dir[0]*= -1;
    dir[1]*= -1;
    dir[2]*= -1;
    return dir;
}


Camera::
Camera()
{
    mPos                = pt::math::float3(0.0f, -4.0f, 0.0f);
    mVecUp              = pt::math::float3(0.0f, 0.0f, 1.0f);
    mLookatRelative     = pt::math::float3(0.0f, 1.0f, 0.0f);
    mFOV                = 75.0f;
    mAspectRatio        = 16.0f/9.0f;
    mClippingNearDist   = 1.0f;
    mClippingFarDist    = 1000.0f;
    UpdateData();
}


void Camera::
UpdateData()
{
    mLookat      = mPos + mLookatRelative;

    mCamZ        = (mPos - mLookat); //note: invert this for DirectX
    assert(0.0f < mCamZ.length());  //TODO: handle gimbal lock with some reset and log error
    mCamZ        = mCamZ.normalize();

    mCamRight    = mVecUp.cross(mCamZ);
    assert(0.0f < mCamRight.length());  //TODO: handle gimbal lock with some reset and log error
    mCamRight    = mCamRight.normalize();
    mCamUp       = mCamZ.cross(mCamRight);  //can skip normalization here

}


pt::math::float4x4 Camera::
GetViewMtx() const
{
    pt::math::float4x4  translation = pt::math::float4x4::identity;
    translation.m[3][0] -= mPos.v[0];
    translation.m[3][1] -= mPos.v[1];
    translation.m[3][2] -= mPos.v[2];

    pt::math::float4x4  orient = pt::math::float4x4::identity;
    orient.m[0][0] = mCamRight.v[0];  orient.m[0][1] = mCamUp.v[0]; orient.m[0][2] = mCamZ.v[0];
    orient.m[1][0] = mCamRight.v[1];  orient.m[1][1] = mCamUp.v[1]; orient.m[1][2] = mCamZ.v[1];
    orient.m[2][0] = mCamRight.v[2];  orient.m[2][1] = mCamUp.v[2]; orient.m[2][2] = mCamZ.v[2];

    return translation * orient;
}


pt::math::float4x4 Camera::
GetProjMtx() const
{
    pt::math::float4x4 proj = pt::math::float4x4::identity;

    float NearZ = mClippingNearDist;
    float FarZ  = mClippingFarDist;

    proj.m[0][0] = -1/  (tanf(mFOV / 2) * mAspectRatio);
    proj.m[1][1] = -1/   tanf(mFOV / 2);

    proj.m[2][2] = (-1*NearZ-FarZ) / (NearZ - FarZ);
    proj.m[2][3] = (2*FarZ*NearZ)  / (NearZ - FarZ);
    proj.m[3][2] = -1.0f;   //OpenGL
    //mProjMtx.m[3][2] = 1.0f;  //DirectX

    proj.m[3][3] = 0;

    return proj;
}


void Camera::
Move(const pt::math::float3& dir)
{
    mPos += dir;
    UpdateData();
}


void Camera::
MoveTarget(float x_angle, float y_angle)
{
    pt::math::float4x4  rot;
    pt::math::float4    target;

    //rotate vertically
    if(y_angle != 0.0f){
        rot     = pt::math::float4x4::rotation(mCamRight, y_angle);
        target  = pt::math::float4(mLookatRelative, 1);
        target  = target * rot;
        mLookatRelative = pt::Vecf3FromVecf4(target);
        UpdateData();
    }

    //rotate horizontally
    if(x_angle != 0.0f){
        rot     = pt::math::float4x4::rotation(mCamUp, x_angle);
        target  = pt::math::float4(mLookatRelative, 1);
        target  = target * rot;
        mLookatRelative = pt::Vecf3FromVecf4(target);
        UpdateData();
    }
}


pt::math::float3 Camera::
GetDir(Camera::Dir direction) const
{
    assert(direction < 6); //TODO: log error instead
    switch (direction){
        case Dir::FORWARD:      return GetForward();
        case Dir::BACKWARD:     return GetBackward();
        case Dir::LEFT:         return GetLeft();
        case Dir::RIGHT:        return GetRight();
        case Dir::UP:           return GetUp();
        case Dir::DOWN:         return GetDown();
    }
    return pt::math::float3::xUnit;
}

float Camera::
GetAspectRatio() const
{
    return mAspectRatio;
}

void Camera::
SetAspectRatio(float ratio)
{
    mAspectRatio = ratio;
}

