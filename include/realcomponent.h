/** -----------------------------------------------------------------------------
  * FILE:    realcomponent.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Components, that have physical boundaries
  *           and/or visual representation.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "worldcomponent.h"

namespace pttoth{
namespace engine{


class RealComponent: public WorldComponent{
    friend class Component;
    static void _RegisterRealComponentParts(RealComponent* component);
    static void _UnregisterRealComponentParts(RealComponent* component);
public:
    RealComponent();
    RealComponent(const RealComponent& other);
    RealComponent(RealComponent&& other);
    virtual ~RealComponent();
    RealComponent& operator=(const RealComponent &other);
    RealComponent& operator=(RealComponent &&other);
    bool operator==(const RealComponent &other)const;

    // Component interface
public:
    virtual void tick(float t, float dt) override;
    virtual void OnRegistered() override;
    virtual void OnUnregistered() override;

    // WorldComponent interface
protected:
    virtual void onSpawn() override;
};


}
}


