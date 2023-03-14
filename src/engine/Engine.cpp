#include "engine/Engine.h"

#include "engine/EngineEvent.h"
#include "engine/Entity.h"
#include "engine/Component.h"
#include "engine/Services.h"
#include "engine/EngineControl.h"

#include "engine/DrawingControl.h"

#include "engine/Scheduler.h"

#include "pt/logging.h"

#include "SDL2/SDL.h"

#include <iostream>
#include <assert.h>

using namespace pt;
using namespace engine;




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
                         //| SDL_INIT_VIDEO
                         //| SDL_INIT_HAPTIC
                         | SDL_INIT_JOYSTICK
                         | SDL_INIT_GAMECONTROLLER
                         );
    if( 0 != init  ){
        //TODO: remove this and everything related to this handling logic!
        const char* errormsg = "Failed to initialize SDL timer";
        SetErrorMessage( errormsg );

        throw std::runtime_error("Failed to initialize SDL");
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

    // there seems to be a problem here ( thread desnyc? )
    //  without the delay, the timer doesn't always start
    SDL_Delay( 150 );

    bool success = mGameTimer.StartNewTimer( 1000.0f / mTickrate ); //ms
    if(!success){
        pt::err << "Engine::OnStart(): Could not initialize game timer in SDL!\n";
    }

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


float Engine::
GetTickrate() const
{
    return mTickrate;
}


void Engine::
SetTickrate(float rate)
{
    mGameTimer.SetInterval( (Uint32) 1000.0f / rate);
    mTickrate = rate;
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
    mTickrate = 60.0f;
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


    mScheduler2.ProcessPendingTasks();
    mScheduler2.TickPrePhysics( ft, fdt );
    mScheduler2.TickDuringPhysics( ft, fdt );
    mScheduler2.TickPostPhysics( ft, fdt );



    // --- TODO: delete this (old impl)
    mScheduler.ProcessPendingTasks();

    mScheduler.TickPrePhysics( ft, fdt );
    mScheduler.TickDuringPhysics( ft, fdt );
    mScheduler.TickPostPhysics( ft, fdt );
    // ---

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
        Quit();
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
{
    auto dc = Services::GetDrawingControl();
    auto dc2 = Services::GetDrawingControl2();
    if( dc2 != nullptr ){
        dc2->DrawScene( t, dt );
    }else{
        if( dc != nullptr ){
            dc->DrawScene( t, dt );
        }
    }
}


//--------------------------------------------------
//--------------------------------------------------
//                  TimerData
//--------------------------------------------------
//--------------------------------------------------


std::vector<Uint64> vec;

Uint32
GenerateGametimerTick(Uint32 interval, void* param)
{
    //timer always reacts at most in 20ms to changes (50 times/sec)
    Uint32 maximum_response_time = 20; //ms

    Engine::GameTimer* ptimer = reinterpret_cast<Engine::GameTimer*>(param);

    Uint64 currentTime = SDL_GetTicks64();
    Engine::GameTimer::State timerstate = ptimer->GetState();

    Uint32 nextUpdateTime = maximum_response_time;

    //if we're on (or past the) time to tick
    if( timerstate.nextTick <= currentTime ){
        timerstate.lastTick = timerstate.nextTick;

        //catch up by skipping missed ticks
        if( timerstate.nextTick + timerstate.interval < currentTime ){
            timerstate.nextTick += currentTime - timerstate.nextTick;
        }
        timerstate.nextTick += timerstate.interval;

        SDL_Event ev;
        SDL_UserEvent userevent;

        userevent.type = SDL_USEREVENT;
        userevent.code = EngineEvent::EV_GAMETIMER_TICK;
        userevent.data1 = NULL;
        userevent.data2 = NULL;

        ev.user = userevent;

        SDL_PushEvent(&ev);
    }

    ptimer->SetState(timerstate);
    timerstate.lastUpdate = currentTime;

    //if next tick is sooner than maximum interval
    if( timerstate.nextTick - currentTime < maximum_response_time ){
        nextUpdateTime = timerstate.nextTick - currentTime; //run again sooner
    }

    return(nextUpdateTime);
}


Engine::GameTimer::
GameTimer()
{}


Engine::GameTimer::
~GameTimer()
{
    //don't destroy while an other thread holds the instance
    //auto-unlock the mutex on destruction
    std::lock_guard<std::mutex> guard(mMutex);
}


Engine::GameTimer::
GameTimer(uint32_t interval):
    GameTimer()
{
    mState.interval = interval;
}


bool Engine::GameTimer::
StartNewTimer(Uint32 interval)
{
    std::lock_guard<std::mutex> guard(mMutex);

    if(mId != 0){
        SDL_RemoveTimer(mId);
        mId = 0;
        mState = GameTimer::State();
    }

    mState.interval = interval;
    mState.startTime = SDL_GetTicks64();

    mId = SDL_AddTimer( mState.interval, GenerateGametimerTick, this );

    if( !IsRunning() ){
        mState = GameTimer::State();
        return false;
    }

    return true;
}


bool Engine::GameTimer::
IsRunning() const
{
    return (0 != mId);
}


Uint64 Engine::GameTimer::
GetUptime() const
{
    std::lock_guard<std::mutex> guard(mMutex);

    if( IsRunning() ){
        Uint64 current_time = SDL_GetTicks64();
        return current_time - mState.startTime;
    }
    return 0;
}


SDL_TimerID Engine::GameTimer::
GetId() const
{
    std::lock_guard<std::mutex> guard(mMutex);
    return mId;
}


void Engine::GameTimer::
SetInterval(Uint32 interval)
{
    std::lock_guard<std::mutex> guard(mMutex);
    mState.interval = interval;
    mState.nextTick = mState.lastTick + interval;
}


Engine::GameTimer::State Engine::GameTimer::
GetState() const
{
    std::lock_guard<std::mutex> guard(mMutex);
    return mState;
}


void Engine::GameTimer::
SetState(const State& state)
{
    std::lock_guard<std::mutex> guard(mMutex);
    mState = state;
}

