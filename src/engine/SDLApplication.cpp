#include "engine/SDLApplication.h"

#include "engine/Services.h"
#include "engine/EngineEvent.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"

SDLApplication::
SDLApplication()
{}


SDLApplication::
SDLApplication(const int argc, char *argv[]):
    mArgc(argc), mArgv(argv)
{}


SDLApplication::
~SDLApplication()
{}


bool SDLApplication::
Initialize()
{
    int init = SDL_Init( SDL_INIT_EVENTS );
    if( 0 != init  ){
        const char* errormsg = "Failed to initialize SDL events";
        SetErrorMessage(errormsg);
        return false;
    }
    atexit(SDL_Quit);
    return true;
}


void SDLApplication::
OnStart()
{}


void SDLApplication::
OnExit()
{}


void SDLApplication::
OnShutdownSignal()
{
    SignalShutdownReady();
}


void SDLApplication::
SignalShutdownReady()
{
    SDL_Event ev;
    BuildUserEvent(&ev,EngineEvent::EV_SHUTDOWN_READY, nullptr, nullptr);
    SDL_PushEvent(&ev);
}


void SDLApplication::
Execute()
{
    //Qt dies for some reason if we don't wait here
    //  before locking the mutex
    SDL_Delay(50);

    std::lock_guard<std::mutex> lock(mMutExec);

    SetExecuting(true);
    OnStart();

    SDL_Event ev;
    while( IsExecuting() ){
        SDL_WaitEvent( &ev );
        switch(ev.type){
            case SDL_USEREVENT:
                if(EngineEvent::EV_SHUTDOWN_BEGIN == ev.user.code){
                    OnShutdownSignal();
                }
                if(EngineEvent::EV_SHUTDOWN_READY == ev.user.code){
                    SetExecuting(false);
                }
        }
        OnEvent( &ev );     //app handle
    }
    OnExit();
}


bool SDLApplication::
IsExecuting()
{
    std::lock_guard<std::mutex> lock(mMutQueryExec);
    return mIsExecuting;
}


std::string SDLApplication::
GetError()
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    return mError;
}


void SDLApplication::
Quit()
{
    SDL_Event ev;
    BuildUserEvent(&ev,EngineEvent::EV_SHUTDOWN_BEGIN, nullptr, nullptr);
    SDL_PushEvent(&ev);
}


void SDLApplication::
SetErrorMessage(char* const msg)
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    mError = msg;
}


void SDLApplication::
SetErrorMessage(const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mMutQueryError);
    mError = msg;
}


void SDLApplication::
SetExecuting(bool val)
{
    std::lock_guard<std::mutex> lock(mMutQueryExec);
    mIsExecuting = val;
}


