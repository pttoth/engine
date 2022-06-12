/** -----------------------------------------------------------------------------
  * FILE:    Scheduler.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Handles the details of updating the engine elements.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Ticker.h"

namespace engine{

class Scheduler
{

public:
    virtual void RegisterTick(Ticker& subject) = 0;
    virtual void UnregisterTick(Ticker& subject) = 0;
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) = 0;
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) = 0;

    virtual void RemoveEntityDependencies(Ticker& subject) = 0;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) = 0;

    virtual void Update() = 0;

};

}
