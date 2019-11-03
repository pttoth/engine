#pragma once

#include <cstdint>
#include "SDL2/SDL_events.h"

namespace PT{
    enum EngineEventCode{
        EV_NO_CODE = 0,
        EV_SHUTDOWN_BEGIN = 1,
        EV_SHUTDOWN_READY = 2,
        EV_ENGINETIMER_TICK = 3,

        //needed?
        EV_ENTITY_DESTROYED,
        EV_COMPONENT_DESTROYED,
    };

    //todo: dump/rewrite this
/*
    bool operator==(game_event lhs, Sint32 rhs){
        return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
    }

    bool operator==(Sint32 lhs, game_event rhs){
        return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
    }
*/
} //end of namespace

