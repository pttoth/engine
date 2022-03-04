#include "camera.h"

using namespace engine;

engine::Camera2D::
Camera2D(): Entity()
{
    mBasicPosComponent.setPosition( pt::math::float3(0.0f, -4.0f, 0.0f) );
    this->SetRootComponent(&mBasicPosComponent);

    mVecUp              = pt::math::float3(0.0f, 0.0f, 1.0f);
    mLookatRelative     = pt::math::float3(0.0f, 1.0f, 0.0f);
    mFOV                = 75.0f;
    mAspectRatio        = 16.0f/9.0f;
    mClippingNearDist   = 1.0f;
    mClippingFarDist    = 1000.0f;



    UpdateData();
}


void Camera2D::
OnRegister()
{}


void Camera2D::
OnUnregister()
{}


void engine::Camera2D::
UpdateData()
{
    const pt::math::float3& pos = mBasicPosComponent.getPosition();

    mLookat      = pos + mLookatRelative;

    mCamZ        = (pos - mLookat); //note: invert this for DirectX
    assert(0.0f < mCamZ.length());  //TODO: handle gimbal lock with some reset and log error
    mCamZ        = mCamZ.normalize();

    mCamRight    = mVecUp.cross(mCamZ);
    assert(0.0f < mCamRight.length());  //TODO: handle gimbal lock with some reset and log error
    mCamRight    = mCamRight.normalize();
    mCamUp       = mCamZ.cross(mCamRight);  //can skip normalization here

}


pt::math::float4x4 engine::Camera2D::
GetViewMtx() const
{
    const pt::math::float3& pos = mBasicPosComponent.getPosition();

    pt::math::float4x4  translation = pt::math::float4x4::identity;
    translation.m[3][0] -= pos.v[0];
    translation.m[3][1] -= pos.v[1];
    translation.m[3][2] -= pos.v[2];

    pt::math::float4x4  orient = pt::math::float4x4::identity;
    orient.m[0][0] = mCamRight.v[0];  orient.m[0][1] = mCamUp.v[0]; orient.m[0][2] = mCamZ.v[0];
    orient.m[1][0] = mCamRight.v[1];  orient.m[1][1] = mCamUp.v[1]; orient.m[1][2] = mCamZ.v[1];
    orient.m[2][0] = mCamRight.v[2];  orient.m[2][1] = mCamUp.v[2]; orient.m[2][2] = mCamZ.v[2];

    return translation * orient;
}


pt::math::float4x4 engine::Camera2D::
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


void engine::Camera2D::
Move(const pt::math::float3& dir)
{
    auto root = this->getRootComponent();

    root->setPosition( root->getPosition() + dir );

    UpdateData();
}


void engine::Camera2D::
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


pt::math::float3 engine::Camera2D::
GetDir(Camera2D::Dir direction) const
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


float engine::Camera2D::
GetAspectRatio() const
{
    return mAspectRatio;
}

void engine::Camera2D::
SetAspectRatio(float ratio)
{
    mAspectRatio = ratio;
}


void Camera2D::
tick(float t, float dt)
{}


pt::math::float3 engine::Camera2D::
GetForward() const
{
    return -mCamZ;
}


pt::math::float3 engine::Camera2D::
GetBackward() const
{
    return mCamZ;
}


pt::math::float3 engine::Camera2D::
GetRight() const
{
    return mCamRight;
}


pt::math::float3 engine::Camera2D::
GetLeft() const
{
    return -mCamRight;
}


pt::math::float3 engine::Camera2D::
GetUp() const
{
    return mCamUp;
}


pt::math::float3 engine::Camera2D::
GetDown() const
{
    return -mCamUp;
}

