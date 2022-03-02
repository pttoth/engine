

#include "sdlapplication.h"

#include "services.h"
//#include "sdlcontrol.h"

#include "game_events.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"

SDLApplication::
        SDLApplication():_argc(0), _argv(nullptr),
                      _is_executing(false){
}

SDLApplication::
        SDLApplication(const int argc,
                    char *argv[]): _argc(argc), _argv(argv),
                                   _is_executing(false){
}

SDLApplication::
    ~SDLApplication(){
}

bool SDLApplication::
        initialize(){
    int init = SDL_Init( SDL_INIT_EVENTS );
    if( 0 != init  ){
        setErrorMessage("Failed to initialize SDL events");
        return false;
    }
    atexit(SDL_Quit);
    return true;
}

void SDLApplication::
        onStart(){
}

void SDLApplication::
        onExit(){
}

void SDLApplication::
        onShutdownSignal(){
    signalShutdownReady();
}

void SDLApplication::
        signalShutdownReady(){
    SDL_Event ev;
    BuildUserEvent(&ev,game_event::EV_SHUTDOWN_READY, nullptr, nullptr);
    SDL_PushEvent(&ev);
}


void SDLApplication::
        execute(){
    //Qt dies for some reason if we don't wait here
    //  before locking the mutex
    SDL_Delay(50);

    std::lock_guard<std::mutex> lock(_mut_exec);

    setExecuting(true);
    onStart();

    SDL_Event ev;
    while( isExecuting() ){
        SDL_WaitEvent( &ev );
        switch(ev.type){
            case SDL_USEREVENT:
                if(game_event::EV_SHUTDOWN_BEGIN == ev.user.code){
                    onShutdownSignal();
                }
                if(game_event::EV_SHUTDOWN_READY == ev.user.code){
                    setExecuting(false);
                }
        }
        onEvent( &ev );     //app handle
    }
    onExit();
}

bool SDLApplication::
        isExecuting(){
    std::lock_guard<std::mutex> lock(_mut_query_exec);
    return _is_executing;
}

std::string SDLApplication::
        getError(){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    return _error;
}

void SDLApplication::
        quit(){
    SDL_Event ev;
    BuildUserEvent(&ev,game_event::EV_SHUTDOWN_BEGIN, nullptr, nullptr);
    SDL_PushEvent(&ev);
}

void SDLApplication::
        setErrorMessage(char* const msg){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    _error = msg;
}

void SDLApplication::
        setErrorMessage(const std::string& msg){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    _error = msg;
}

void SDLApplication::
        setExecuting(bool val){
    std::lock_guard<std::mutex> lock(_mut_query_exec);
    _is_executing = val;
}


