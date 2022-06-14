/** -----------------------------------------------------------------------------
  * FILE:    PositionComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component with just a world coordinate.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "WorldComponent.h"

namespace engine{


class PositionComponent: public WorldComponent
{
public:
    PositionComponent(const std::string& name);
    PositionComponent(const PositionComponent& other);
    PositionComponent(PositionComponent&& other) = delete;
    virtual ~PositionComponent();
    PositionComponent& operator=(const PositionComponent &other) = delete;
    PositionComponent& operator=(PositionComponent &&other) = delete;
    bool operator==(const PositionComponent &other);

public:
    virtual void tick(float t, float dt) override;

protected:
    virtual void OnSpawn() override;
    virtual void OnDespawn() override;

};

}
