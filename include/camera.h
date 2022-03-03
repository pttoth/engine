#pragma once

#include "engine/mathhelpers.hpp"
#include <assert.h>

namespace pt{
namespace engine{

class Camera{
private:
    //cached direction data
    math::float3    camZ;
    math::float3    camRight;
    math::float3    camUp;

    math::float3    m_pos;     //camera position
    math::float3    m_lookat;  //camera target coordinates
    math::float3    m_lookat_relative;
    math::float3    m_vup;     //peferred vertical direction (this is mostly {0,0,1} )

    float           m_aspect_ratio;

    float           m_FOV;
    float           m_clipping_near_dist;
    float           m_clipping_far_dist;

    math::float3 getForward() const;
    math::float3 getBackward() const;
    math::float3 getRight() const;
    math::float3 getLeft() const;
    math::float3 getUp() const;
    math::float3 getDown() const;

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
    math::float4x4 getViewMtx();
    math::float4x4  getProjMtx();
    void move(math::float3 dir);
    void moveTarget(float x_angle, float y_angle);
    math::float3 getDir(Dir direction) const;
};

} //end of namespace engine
} //end of namespace PT

