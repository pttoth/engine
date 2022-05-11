/** -----------------------------------------------------------------------------
  * FILE:    EngineControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A reduced interface, that game elements can access to communicate
  *           with the engine, without compromising critical functionality by
  *           letting uncontrolled access to critical functions.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"
#include "Component.h"
#include "Entity.h"

namespace engine{

class EngineControl{

public:
    virtual void RegisterEntity(Entity* e) = 0;
    virtual void UnregisterEntity(Entity* e) = 0;
    virtual void RegisterComponent(Component* c) = 0;
    virtual void UnregisterComponent(Component* c) = 0;

    virtual void RegisterTick(Entity* e) = 0;
    virtual void UnregisterTick(Entity* e) = 0;
    virtual void AddTickDependency(Entity* subject, Entity* dependency) = 0;
    virtual void RemoveTickDependency(Entity* subject, Entity* dependency) = 0;

    virtual void RemoveEntityDependencies(Entity* subject) = 0;
    virtual void RemoveDependenciesReferencingEntity(Entity* dependency) = 0;

};

}
