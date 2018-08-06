#pragma once

#include <cstdint>
#include "SDL2/SDL_events.h"

namespace pttoth{
        inline void BuildUserEvent(SDL_Event* event,
                            int const code,
                            void* subject,
                            void* data){
        SDL_UserEvent usev;
        usev.type = SDL_USEREVENT;
        usev.code = code;
        usev.data1 = subject;
        usev.data2 = data;

        event->user = usev;
    }

    enum game_event{
        EV_NO_CODE = 0,
        EV_APP_QUIT,
        EV_GAMETIMER_TICK,

        EV_OBJECT_DESTROYED,
        EV_OBJECT_MOVED,
    };
/*
    bool operator==(game_event lhs, Sint32 rhs){
        return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
    }

    bool operator==(Sint32 lhs, game_event rhs){
        return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
    }
*/
}

