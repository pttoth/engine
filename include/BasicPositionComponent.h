/** -----------------------------------------------------------------------------
  * FILE:    BasicPositionComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component with just a world coordinate.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "worldcomponent.h"

namespace engine{


class BasicPositionComponent: public WorldComponent
{
public:
    BasicPositionComponent();
    BasicPositionComponent(const BasicPositionComponent& other) = delete;
    BasicPositionComponent(BasicPositionComponent&& other) = delete;
    virtual ~BasicPositionComponent();
    BasicPositionComponent& operator=(const BasicPositionComponent &other) = delete;
    BasicPositionComponent& operator=(BasicPositionComponent &&other) = delete;
    bool operator==(const BasicPositionComponent &other);

public:
    virtual void tick(float t, float dt) override;
    virtual void OnRegistered() override;
    virtual void OnUnregistered() override;

protected:
    virtual void OnSpawn() override;
};

}
