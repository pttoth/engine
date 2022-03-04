/** -----------------------------------------------------------------------------
  * FILE:    enginecontrol.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A reduced interface, that game elements can access to communicate
  *           with the engine, without compromising critical functionality by
  *           letting uncontrolled access to critical functions.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Camera.h"
#include "component.h"
#include "entity.h"

namespace engine{

class EngineControl{

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

    //virtual void SetMainCamera(Camera* camera) = 0;
    //virtual Camera* GetMainCamera() = 0;

private:
    //Camera*  mMainCamera;
};

}
