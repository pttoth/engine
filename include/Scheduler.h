/** -----------------------------------------------------------------------------
  * FILE:    Scheduler.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Interface for classes that handle updating elements each frame.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Ticker.h"

namespace engine{

class Scheduler
{

public:
    /**
     * @brief AddTicker:
     *   Registers Ticker to have its Tick() function called each frame.
     * @note Only takes effect at the start of the next frame.
     */
    virtual void AddTicker(Ticker& subject) = 0;


    /**
     * @brief RemoveTicker:
     *   Unregisters Ticker to not have its Tick() function called each frame.
     * @note Only takes effect at the start of the next frame.
     */
    virtual void RemoveTicker(Ticker& subject) = 0;


    /**
     * @brief AddTickDependency:
     *    Guarantees, that 'subject' will only tick after 'dependency' has finished ticking for the current frame.
     * @note  'subject' and 'dependency' has to be in the same TickGroup.
     *        Only takes effect at the start of the next frame.
     */
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) = 0;


    /**
     * @brief RemoveTickDependency:
     *    Removes constraint for 'subject' to only tick after 'dependency' has finished ticking for the current frame.
     * @note  'subject' and 'dependency' has to be in the same TickGroup.
     *        Only takes effect at the start of the next frame.
     */
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) = 0;

    /**
     * @brief RemoveDependenciesForTicker:
     *   Removes all dependency constraints for 'subject'.
     */
    virtual void RemoveDependenciesForTicker(Ticker& subject) = 0;

    /**
     * @brief RemoveDependenciesReferencingTicker:
     *   Removes all dependency constraints that marks 'dependency' as target.
     */
    virtual void RemoveDependenciesReferencingTicker(Ticker& dependency) = 0;

};

}
