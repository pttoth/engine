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
    virtual void RegisterEntity(Entity& entity) = 0;
    virtual void UnregisterEntity(Entity& entity) = 0;
    virtual void RegisterComponent(Component& component) = 0;
    virtual void UnregisterComponent(Component& component) = 0;

    virtual void RegisterTick(Ticker& subject) = 0;
    virtual void UnregisterTick(Ticker& subject) = 0;
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) = 0;
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) = 0;

    virtual void RemoveEntityDependencies(Ticker& subject) = 0;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) = 0;

};

}
