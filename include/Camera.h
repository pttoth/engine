#pragma once

#include "Entity.h"

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

    Camera(const std::string& name);
    Camera(const Camera& other);
    Camera(Camera&& other) = delete;
    virtual ~Camera(){}
    Camera& operator=(const Camera &other) = delete;
    Camera& operator=(Camera &&other) = delete;
    bool operator==(const Camera &other) const;

    virtual void        OnRegister() = 0;
    virtual void        OnUnregister() = 0;

    // note: expects line vectors
    virtual const pt::math::float4x4  GetViewMtx() const = 0;
    virtual const pt::math::float4x4  GetProjMtx() const = 0;
    virtual const pt::math::float3    GetDir(Dir direction) const;

    virtual void    Move(const pt::math::float3& dir) = 0;
    virtual float   GetAspectRatio() const;
    virtual void    SetAspectRatio(float ratio);
    virtual float   GetZoom() const;
    virtual void    SetZoom(float zoom);
protected:
    virtual const pt::math::float3  GetForward() const = 0;
    virtual const pt::math::float3  GetBackward() const = 0;
    virtual const pt::math::float3  GetRight() const = 0;
    virtual const pt::math::float3  GetLeft() const = 0;
    virtual const pt::math::float3  GetUp() const = 0;
    virtual const pt::math::float3  GetDown() const = 0;

private:
    void    InitMembers();

    float   mAspectRatio;
    float   mZoom;



protected:

};

} //end of namespace engine

