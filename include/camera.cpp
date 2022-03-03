#include "engine/camera.h"

using namespace pt;
using namespace engine;

math::float3 Camera::
getForward() const{
    math::float3 dir = camZ;
    dir[0]*= -1;
    dir[1]*= -1;
    dir[2]*= -1;
    return dir;
}
math::float3 Camera::
getBackward() const{
    math::float3 dir = camZ;
    return dir;
}
math::float3 Camera::
getRight() const{
        math::float3 dir = camRight;
        return dir;
}
math::float3 Camera::
getLeft() const{
        math::float3 dir = camRight;
        dir[0]*= -1;
        dir[1]*= -1;
        dir[2]*= -1;
        return dir;
}
math::float3 Camera::
getUp() const{
        math::float3 dir = camUp;
        return dir;
}
math::float3 Camera::
getDown() const{
        math::float3 dir = camUp;
        dir[0]*= -1;
        dir[1]*= -1;
        dir[2]*= -1;
        return dir;
}

Camera::
Camera(){
    m_pos                   = math::float3(0.0f, -4.0f, 0.0f);
    m_vup                   = math::float3(0.0f, 0.0f, 1.0f);
    m_lookat_relative       = math::float3(0.0f, 1.0f, 0.0f);
    m_FOV                   = 75.0f;
    m_aspect_ratio          = 16.0f/9.0f;
    m_clipping_near_dist    = 1.0f;
    m_clipping_far_dist     = 1000.0f;
    updateData();
}

void Camera::
updateData(){
        m_lookat = m_pos + m_lookat_relative;

        camZ        = (m_pos - m_lookat); //note: invert this for DirectX
        assert(0.0f < camZ.length());
        camZ        = camZ.normalize();

        camRight    = m_vup.cross(camZ);
        assert(0.0f < camRight.length());
        camRight    = camRight.normalize();
        camUp       = camZ.cross(camRight);  //can skip normalization here

        return;
}

math::float4x4 Camera::
getViewMtx(){
        updateData();

        math::float4x4  translation = math::float4x4::identity;
        translation.m[3][0] -= m_pos.v[0];
        translation.m[3][1] -= m_pos.v[1];
        translation.m[3][2] -= m_pos.v[2];

        math::float4x4  orient = math::float4x4::identity;
        orient.m[0][0] = camRight.v[0];  orient.m[0][1] = camUp.v[0]; orient.m[0][2] = camZ.v[0];
        orient.m[1][0] = camRight.v[1];  orient.m[1][1] = camUp.v[1]; orient.m[1][2] = camZ.v[1];
        orient.m[2][0] = camRight.v[2];  orient.m[2][1] = camUp.v[2]; orient.m[2][2] = camZ.v[2];

        return translation * orient;
}

math::float4x4 Camera::
getProjMtx(){
        math::float4x4 proj = math::float4x4::identity;

        float NearZ = m_clipping_near_dist;
        float FarZ  = m_clipping_far_dist;

        proj.m[0][0] = -1/  (tanf(m_FOV / 2) * m_aspect_ratio);
        proj.m[1][1] = -1/   tanf(m_FOV / 2);

        proj.m[2][2] = (-1*NearZ-FarZ) / (NearZ - FarZ);
        proj.m[2][3] = (2*FarZ*NearZ)  / (NearZ - FarZ);
        proj.m[3][2] = -1.0f;   //OpenGL
        //proj.m[3][2] = 1.0f;  //DirectX

        proj.m[3][3] = 0;

        return proj;
}

void Camera::
move(math::float3 dir){
        m_pos += dir;
        //std::cout << "-----moving camera-----\n";
        updateData();
        //    std::cout << "-----------------------\n";
}

void Camera::
moveTarget(float x_angle, float y_angle){
        math::float4x4  rot;
        math::float4    target;
        //rotate vertically
        if(y_angle != 0.0f){
            rot     = math::float4x4::rotation(camRight, y_angle);
            target  = math::float4(m_lookat_relative, 1);
            target  = target * rot;
            m_lookat_relative = Vecf3FromVecf4(target);
            updateData();
        }

        //rotate horizontally
        if(x_angle != 0.0f){
            rot     = math::float4x4::rotation(camUp, x_angle);
            target  = math::float4(m_lookat_relative, 1);
            target  = target * rot;
            m_lookat_relative = Vecf3FromVecf4(target);
            updateData();
        }
}

math::float3 Camera::
getDir(Camera::Dir direction) const{
        assert(direction < 6);
        switch (direction){
        case Dir::FORWARD:      return getForward();
        case Dir::BACKWARD:     return getBackward();
        case Dir::LEFT:         return getLeft();
        case Dir::RIGHT:        return getRight();
        case Dir::UP:           return getUp();
        case Dir::DOWN:         return getDown();
        }
        return math::float3::xUnit; //TODO: build error string and throw exception instead
    }



