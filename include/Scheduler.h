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
    //TODO AddScheduler: fix comments

    /**
     * @brief registerTick:
     *          Registers Entity to have its tick() function called during frames
     * @note  only takes effect at the start of the next frame
     */
    virtual void RegisterTick(Ticker& subject) = 0;


    /**
     * @brief unregisterTick:
     *          Unregisters Entity to not have its tick() function called during frames
     * @note  only takes effect at the start of the next frame
     */
    virtual void UnregisterTick(Ticker& subject) = 0;


    /**
     * @brief addTickDependency:
     *          Ensures, that 'subject' will only tick after 'dependecy' has ticked in the same TickGroup
     *          'subject' and 'dependency' has to be in the same TickGroup
     * @note  only takes effect at the start of the next frame
     */
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) = 0;


    /**
     * @brief removeTickDependency:
     *          Removes ensurance, that 'subject' will only tick after 'dependecy' has ticked in the same TickGroup
     * @note  only takes effect at the start of the next frame
     */
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) = 0;

    virtual void RemoveEntityDependencies(Ticker& subject) = 0;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) = 0;

};

}
