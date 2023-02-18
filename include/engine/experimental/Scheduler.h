#pragma once

#include "engine/experimental/Actor.h"

#include <cstdint>

namespace engine{
namespace experimental{

class Scheduler
{
public:
    /**
     * @brief AddActor:
     *   Registers Actor to have its Tick() function called each frame.
     * @note Only takes effect at the start of the next frame.
     */
    virtual void AddActor( Actor& subject ) = 0;


    /**
     * @brief RemoveActor:
     *   Unregisters Actor to not have its Tick() function called each frame.
     * @note Only takes effect at the start of the next frame.
     */
    virtual void RemoveActor( Actor& subject ) = 0;


    /**
     * @brief AddTickDependency:
     *    Guarantees, that 'subject' will only tick after 'dependency' has finished ticking for the current frame.
     * @note  'subject' and 'dependency' has to be in the same TickGroup.
     *        Only takes effect at the start of the next frame.
     */
    virtual void AddTickDependency( Actor& subject, Actor& dependency ) = 0;


    /**
     * @brief RemoveTickDependency:
     *    Removes constraint for 'subject' to only tick after 'dependency' has finished ticking for the current frame.
     * @note  'subject' and 'dependency' has to be in the same TickGroup.
     *        Only takes effect at the start of the next frame.
     */
    virtual void RemoveTickDependency( Actor& subject, Actor& dependency ) = 0;

    /**
     * @brief RemoveDependenciesForActor:
     *   Removes all dependency constraints for 'subject'.
     */
    virtual void RemoveDependenciesForActor( Actor& subject ) = 0;

    /**
     * @brief RemoveDependenciesReferencingActor:
     *   Removes all dependency constraints that marks 'dependency' as target.
     */
    virtual void RemoveDependenciesReferencingActor( Actor& dependency ) = 0;


    virtual void TickAllActors() = 0;

protected:

    struct TickUnit{
        Actor*      actor;
        uint64_t    lastTick;
        float       interval;
    };

private:
};

} //end of namespace experimental
} //end of namespace engine
