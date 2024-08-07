#pragma once

#include "SDL2/SDL_events.h"
#include <cstdint>

inline void
BuildUserEvent(SDL_Event* event,
               int const code,
               void* subject,
               void* data)
{
    SDL_UserEvent usev;
    usev.type = SDL_USEREVENT;
    usev.code = code;
    usev.data1 = subject;
    usev.data2 = data;

    event->user = usev;
}

enum EngineEvent
{
    EV_NO_CODE = 0,
    EV_SHUTDOWN_BEGIN = 1,
    EV_SHUTDOWN_READY = 2,

    //EV_OBJECT_DESTROYED,
    //EV_OBJECT_MOVED,
};

//TODO: cleanup
/*
bool operator==(game_event lhs, Sint32 rhs){
    return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
}

bool operator==(Sint32 lhs, game_event rhs){
    return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
}
*/
