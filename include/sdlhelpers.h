#pragma once

namespace PT{

inline void
BuildUserEvent(SDL_Event* event,
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

} // end of namespace PT



