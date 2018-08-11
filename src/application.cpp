

#include "application.h"

#include "services.h"
//#include "sdlcontrol.h"

#include "game_events.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"

using namespace pttoth;

Application::
        Application():_argc(0), _argv(nullptr),
                      _is_executing(false){
}

Application::
        Application(const int argc,
                    char *argv[]): _argc(argc), _argv(argv),
                                   _is_executing(false){
}

Application::
    ~Application(){
}

bool Application::
        initialize(){
    int init = SDL_Init( SDL_INIT_EVENTS );
    if( 0 != init  ){
        setErrorMessage("Failed to initialize SDL events");
        return false;
    }
    atexit(SDL_Quit);
    return true;
}

void Application::
        onStart(){
}

void Application::
        onExit(){
}

void Application::
        onShutdownSignal(){
    signalShutdownReady();
}

void Application::
        signalShutdownReady(){
    SDL_Event ev;
    BuildUserEvent(&ev,game_event::EV_SHUTDOWN_READY, nullptr, nullptr);
    SDL_PushEvent(&ev);
}


void Application::
        execute(){
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

bool Application::
        isExecuting(){
    std::lock_guard<std::mutex> lock(_mut_query_exec);
    return _is_executing;
}

std::string Application::
        getError(){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    return _error;
}

void Application::
        quit(){
    SDL_Event ev;
    BuildUserEvent(&ev,game_event::EV_SHUTDOWN_BEGIN, nullptr, nullptr);
    SDL_PushEvent(&ev);
}

void Application::
        setErrorMessage(char* const msg){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    _error = msg;
}

void Application::
        setErrorMessage(const std::string& msg){
    std::lock_guard<std::mutex> lock(_mut_query_error);
    _error = msg;
}

void Application::
        setExecuting(bool val){
    std::lock_guard<std::mutex> lock(_mut_query_exec);
    _is_executing = val;
}


