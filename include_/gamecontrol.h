/** -----------------------------------------------------------------------------
  * FILE:    gamecontrol.h
  * AUTHOR:  pttoth - 2018.08.07.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A reduced interface, that game elements can access to communicate
  *           with the engine, without compromising critical functionality by
  *           letting uncontrolled access to critical functions.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "component.h"
#include "entity.h"

namespace pttoth{
namespace engine{

class Entity;
class Component;

class GameControl{

public:
    virtual void registerEntity(Entity* e) = 0;
    virtual void unregisterEntity(Entity* e) = 0;
    virtual void registerComponent(Component* c) = 0;
    virtual void unregisterComponent(Component* c) = 0;

    virtual void registerTick(Entity* e) = 0;
    virtual void unregisterTick(Entity* e) = 0;
    virtual void addTickDependency(Entity* subject, Entity* dependency) = 0;
    virtual void removeTickDependency(Entity* subject, Entity* dependency) = 0;

    virtual void removeEntityDependencies(Entity* subject) = 0;
    virtual void removeDependenciesReferencingEntity(Entity* dependency) = 0;

};

} }
