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
    void updateData();
    // note: expects line vectors
    pt::math::float4x4 getViewMtx();
    pt::math::float4x4  getProjMtx();
    void move(pt::math::float3 dir);
    void moveTarget(float x_angle, float y_angle);
    pt::math::float3 getDir(Dir direction) const;

private:
    pt::math::float3 getForward() const;
    pt::math::float3 getBackward() const;
    pt::math::float3 getRight() const;
    pt::math::float3 getLeft() const;
    pt::math::float3 getUp() const;
    pt::math::float3 getDown() const;

    //cached direction data
    pt::math::float3    camZ;
    pt::math::float3    camRight;
    pt::math::float3    camUp;

    pt::math::float3    m_pos;     //camera position
    pt::math::float3    m_lookat;  //camera target coordinates
    pt::math::float3    m_lookat_relative;
    pt::math::float3    m_vup;     //peferred vertical direction (this is mostly {0,0,1} )

    float           m_aspect_ratio;

    float           m_FOV;
    float           m_clipping_near_dist;
    float           m_clipping_far_dist;

};

} //end of namespace engine

