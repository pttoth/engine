/** -----------------------------------------------------------------------------
  * FILE:    DrawableComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Base component for anything with a visual representation.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "realcomponent.h"

namespace engine{


class DrawableComponent: public RealComponent
{
public:
    DrawableComponent(){}
    DrawableComponent(const DrawableComponent& other) = delete;
    DrawableComponent(DrawableComponent&& other) = delete;
    virtual ~DrawableComponent(){}
    DrawableComponent& operator=(const DrawableComponent &other) = delete;
    DrawableComponent& operator=(DrawableComponent &&other) = delete;
    bool operator==(const DrawableComponent &other);

    virtual void Draw() = 0;

    virtual void OnRegistered() override;
    virtual void OnUnregistered() override;
    //virtual void tick(float t, float dt) override;
    //virtual void OnSpawn() override;

protected:
    virtual void OnSpawn() override;
    virtual void OnDespawn() override;

    virtual void OnCreateContext();
    virtual void OnDestroyContext();

};

} //end of namespace
