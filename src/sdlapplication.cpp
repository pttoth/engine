#include "sdlapplication.h"

#include "services.h"

#include "engineevent.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"

SDLApplication::
SDLApplication():mArgc(0), mArgv(nullptr),
                 mIsExecuting(false)
{}

SDLApplication::
SDLApplication(const int argc, char *argv[]):
    mArgc(argc), mArgv(argv),
    mIsExecuting(false)
{}

SDLApplication::
~SDLApplication()
{}

bool SDLApplication::
initialize()
{
    int init = SDL_Init( SDL_INIT_EVENTS );
    if( 0 != init  ){
        setErrorMessage("Failed to initialize SDL events");
        return false;
    }
    atexit(SDL_Quit);
    return true;
}

void SDLApplication::
onStart()
{}

void SDLApplication::
onExit()
{}

void SDLApplication::
onShutdownSignal()
{
    signalShutdownReady();
}

void SDLApplication::
signalShutdownReady()
{
    SDL_Event ev;
    BuildUserEvent(&ev,EngineEvent::EV_SHUTDOWN_READY, nullptr, nullptr);
    SDL_PushEvent(&ev);
}


void SDLApplication::
execute()
{
    //Qt dies for some reason if we don't wait here
    //  before locking the mutex
    SDL_Delay(50);

    std::lock_guard<std::mutex> lock(mMutExec);

    setExecuting(true);
    onStart();

    SDL_Event ev;
    while( isExecuting() ){
        SDL_WaitEvent( &ev );
        switch(ev.type){
            case SDL_USEREVENT:
                if(EngineEvent::EV_SHUTDOWN_BEGIN == ev.user.code){
                    onShutdownSignal();
                }
                if(EngineEvent::EV_SHUTDOWN_READY == ev.user.code){
                    setExecuting(false);
                }
        }
        onEvent( &ev );     //app handle
    }
    onExit();
}

bool SDLApplication::
isExecuting()
{
    std::lock_guard<std::mutex> lock(mMutQueryExec);
    return mIsExecuting;
}

std::string SDLApplication::
getError()
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    return mError;
}

void SDLApplication::
quit()
{
    SDL_Event ev;
    BuildUserEvent(&ev,EngineEvent::EV_SHUTDOWN_BEGIN, nullptr, nullptr);
    SDL_PushEvent(&ev);
}

void SDLApplication::
setErrorMessage(char* const msg)
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    mError = msg;
}

void SDLApplication::
setErrorMessage(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    mError = msg;
}

void SDLApplication::
setExecuting(bool val)
{
    std::lock_guard<std::mutex> lock(mMutQueryExec);
    mIsExecuting = val;
}


