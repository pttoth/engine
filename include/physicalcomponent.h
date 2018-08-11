/** -----------------------------------------------------------------------------
  * FILE:    physicalcomponent.h
  * AUTHOR:  pttoth - 2018.08.10.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Components, that have physical boundaries and/or renderable.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "worldcomponent.h"

namespace pttoth{
namespace engine{


class PhysicalComponent: public WorldComponent{
    friend class Component;
    static void _RegisterPhysicalComponent(PhysicalComponent* component);
    static void _UnregisterPhysicalComponent(PhysicalComponent* component);
public:
    PhysicalComponent();
    PhysicalComponent(const PhysicalComponent& other);
    PhysicalComponent(PhysicalComponent&& other);
    virtual ~PhysicalComponent();
    PhysicalComponent& operator=(const PhysicalComponent &other);
    PhysicalComponent& operator=(PhysicalComponent &&other);
    bool operator==(const PhysicalComponent &other)const;
};


}
}


