#include "Engine.h"

#include <assert.h>
#include "EngineEvent.h"

#include "Entity.h"
#include "Component.h"

#include "Services.h"
#include "EngineControl.h"

#include <iostream>

#include "SDL2/SDL.h"

#include "pt/logging.h"

using namespace pt;
using namespace engine;


Uint32
generate_gametimer_tick(Uint32 interval, void *param)
{
    SDL_Event ev;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = EngineEvent::EV_GAMETIMER_TICK;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    ev.user = userevent;

    SDL_PushEvent(&ev);
    return(interval);
}


Engine::
Engine():
    SDLApplication()
{
    Construct();
}


Engine::
Engine(int const argc, char* argv[]):
    SDLApplication(argc, argv)
{
    Construct();
}


void Engine::
Construct()
{
    mCfgPath= std::string("../../cfg/Engine.cfg");
    InitializeConfig();
}


Engine::
~Engine()
{}


void Engine::
OnStart()
{
    SDLApplication::OnStart();
    int init = SDL_Init( SDL_INIT_EVENTS
                         | SDL_INIT_TIMER
                         | SDL_INIT_AUDIO
                         | SDL_INIT_VIDEO
                         //| SDL_INIT_HAPTIC
                         | SDL_INIT_JOYSTICK
                         | SDL_INIT_GAMECONTROLLER
                         );
    if( 0 != init  ){
        const char* errormsg = "Failed to initialize SDL timer";
        setErrorMessage( errormsg );
    }
    mUptime = SDL_GetTicks();

    atexit( SDL_Quit );

    Services::SetDrawingControl( &mDrawingManager );
    Services::SetScheduler( &mScheduler );

    Services::SetWorld( &mWorld );

    //configure variables
    bool successful_read = ReadConfig();
    if( !successful_read ){
        std::cout << "warning: could not read config file: "
                  << mCfgPath << std::endl;
        SetDefaultSettings();
    }

    Uint32 interval = (Uint32) ( 1000.0f / mTickrate );
    // there seems to be a problem here ( thread desnyc? )
    //  without the delay, the timer doesn't always start
    SDL_Delay( 150 );
    mGametimerId = SDL_AddTimer( interval, generate_gametimer_tick, nullptr );

}


void Engine::
OnExit()
{
    if( 0 != mGametimerId ){
        SDL_RemoveTimer( mGametimerId );
        mGametimerId = 0;
    }

    const World* const w = Services::GetWorld();
    if( w == &mWorld ){
        Services::SetWorld( nullptr );
    }

    const Scheduler* const sched = Services::GetScheduler();
    if( sched == &mScheduler ){
        Services::SetScheduler( nullptr );
    }

    const DrawingControl* const dc = Services::GetDrawingControl();
    if( dc == &mDrawingManager ){
        Services::SetDrawingControl( nullptr );
    }

    SDLApplication::OnExit();
}


void Engine::
OnShutdownSignal()
{
    SignalShutdownReady();
}


void Engine::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseMotion(int32_t x, int32_t y,
              int32_t x_rel, int32_t y_rel,
              uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void Engine::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void Engine::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void Engine::
OnTouchInputEvent()
{
    assert( false );
}


void Engine::
InitializeConfig()
{
    mCfg.setPath( mCfgPath );
    CfgAddKey( mCfg, iTickRate );
}


void Engine::
SetDefaultSettings()
{
    mTickrate = 50;
    mCfg.setI( iTickRate, mTickrate );
}


bool Engine::
ReadConfig()
{
    try{
        mCfg.read();
        int tickrate = mCfg.getI( iTickRate );

        //by now, all reads were successful,
        //  we can start setting the variables
        mTickrate = tickrate;

    }catch(...){
        return false;
    }
    return true;
}



void Engine::
ProcessGameTimerEvent()
{
    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - mUptime) / 1000.0f;
    mUptime = current_time;


    mScheduler.ProcessPendingTasks();

    mScheduler.TickPrePhysics( ft, fdt );
    mScheduler.TickDuringPhysics( ft, fdt );
    mScheduler.TickPostPhysics( ft, fdt );

    Update( ft,fdt );


    // this will happen on a different thread, won't access anything here
    drawScene( ft, fdt );
}



void Engine::
OnEvent(SDL_Event* event)
{
    SDL_Event ev = *event; //avoid unneccessary repeat of dereferences
    switch( ev.type ){
    case SDL_MOUSEMOTION:
        if(! (ev.motion.which & SDL_TOUCH_MOUSEID) ){ //if not touchpad event
            OnMouseMotion( ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel, ev.motion.timestamp, ev.motion.which );
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        OnMouseButtonDown( ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which );
        break;
    case SDL_MOUSEBUTTONUP:
        OnMouseButtonUp( ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which );
        break;
    case SDL_MOUSEWHEEL:
        OnMouseWheel( ev.wheel.x, ev.wheel.y, ev.wheel.timestamp, ev.wheel.which, ev.wheel.direction );
        break;
    case SDL_KEYDOWN:
        OnKeyDown( ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat );
        break;
    case SDL_KEYUP:
        OnKeyUp( ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat );
        break;
    case SDL_FINGERMOTION:
        //TODO: handle...
        break;
    case SDL_FINGERDOWN:
        //TODO: handle...
        break;
    case SDL_FINGERUP:
        //TODO: handle...
        break;
    case SDL_USEREVENT:
        switch( ev.user.code ){
        case EngineEvent::EV_GAMETIMER_TICK:
            ProcessGameTimerEvent();
            break;
        default:
            break;
        }
        break;
    case SDL_QUIT:
        quit();
        break;
    }
}


//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------


void Engine::
drawScene(float t, float dt)
{}

