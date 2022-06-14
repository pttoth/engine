/** -----------------------------------------------------------------------------
  * FILE:    DrawableComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Base component for anything with a visual representation.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "RealComponent.h"

namespace engine{


class DrawableComponent: public RealComponent
{
public:
    DrawableComponent(const std::string& name);
    DrawableComponent(const DrawableComponent& other);
    DrawableComponent(DrawableComponent&& other) = delete;
    virtual ~DrawableComponent();
    DrawableComponent& operator=(const DrawableComponent &other) = delete;
    DrawableComponent& operator=(DrawableComponent &&other) = delete;
    bool operator==(const DrawableComponent &other);

    virtual void Spawn() override;
    virtual void Despawn() override;

    virtual void Draw(float t, float dt) = 0;

    //virtual void tick(float t, float dt) override;
    //virtual void OnSpawn() override;

    void CreateContext();
    void DestroyContext();

protected:
    virtual void OnCreateContext();
    virtual void OnDestroyContext();
private:
    bool    mContextInitialized = false;
};

} //end of namespace
