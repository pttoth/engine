#include "engine/Engine.h"

#include "engine/Component.h"
#include "engine/DrawingControl.h"
#include "engine/EngineEvent.h"
#include "engine/Scheduler.h"
#include "engine/SDLControl.h"
#include "engine/Services.h"
#include "SDLWrapper.h"

#include "pt/logging.h"

#include "SDL2/SDL.h"

#include <assert.h>
#include <iostream>

using namespace pt;
using namespace engine;

Uint32 engine::Engine::mUserEventType = 0;

//--------------------------------------------------
//  TimerData
//--------------------------------------------------

std::vector<Uint64> vec;

Uint32 Engine::GameTimer::
TimerCallback( Uint32 interval, void *param )
{
    Uint64 currentTime = SDL_GetTicks64();
    Engine::GameTimer* ptimer = reinterpret_cast<Engine::GameTimer*>(param);

    Engine::GameTimer::State timerstate = ptimer->GetState();

    //TODO: delete?
    Uint32 nextUpdateTime = mMaximumResponseTime;

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
    StopTimer();

    //don't destroy while an other thread holds the instance
    MutexLockGuard guard( mMutex );
}


bool Engine::GameTimer::
StartNewTimer( Uint32 tickInterval )
{
    MutexLockGuard guard( mMutex );

    Uint64 t_start = SDL_GetTicks64();
    Uint32 timerInterval = 0;
    if( tickInterval < mMaximumResponseTime ){
        timerInterval = tickInterval;
    }else{
        timerInterval = mMaximumResponseTime;
    }

    StopTimer_NoLock();

    SDL_TimerID id = SDL_AddTimer( timerInterval, GameTimer::TimerCallback, this );
    assert( 0 < id );
    if( id <= 0 ){
        pt::log::err << "Failed to create Game Timer!\n";
        return false;
    }

    mState.startTime        = t_start;
    mState.timerInterval    = timerInterval;
    mState.tickInterval     = tickInterval;
    mState.lastTimerUpdate  = t_start;
    mState.lastTick         = t_start;
    mState.nextTick         = t_start + tickInterval;
    mState.timerActive      = true;

    return true;
}


bool Engine::GameTimer::
StopTimer()
{
    MutexLockGuard guard( mMutex );
    StopTimer_NoLock();
}


bool Engine::GameTimer::
IsRunning() const
{
    MutexLockGuard guard( mMutex );
    return IsRunning_NoLock();
}


Uint64 Engine::GameTimer::
GetUptime() const
{
    MutexLockGuard guard(mMutex);

    if( IsRunning_NoLock() ){
        Uint64 current_time = SDL_GetTicks64();
        return current_time - mState.startTime;
    }
    return 0;
}


SDL_TimerID Engine::GameTimer::
GetId() const
{
    MutexLockGuard guard(mMutex);
    return mId;
}


void Engine::GameTimer::
SetInterval(Uint32 interval)
{
    MutexLockGuard guard(mMutex);
    mState.interval = interval;
    mState.nextTick = mState.lastTick + interval;
}


Engine::GameTimer::State Engine::GameTimer::
GetState() const
{
    MutexLockGuard guard(mMutex);
    return mState;
}


void Engine::GameTimer::
SetState(const State& state)
{
    MutexLockGuard guard(mMutex);
    mState = state;
}


bool Engine::GameTimer::
StopTimer_NoLock()
{
    assert( 0 <= mId );
    if( mId != 0 ){
        bool suc = SDL_RemoveTimer( mId );
        if( !suc ){
            pt::log::err << "Failed to remove Game Timer (id='" << mId << "')!\n";
        }
        mId = 0;
        mState = GameTimer::State();
    }
}


bool Engine::GameTimer::
IsRunning_NoLock() const
{
    return mState.timerActive;
}


//--------------------------------------------------
//  Engine
//--------------------------------------------------


Uint32 Engine::
GetUserEventType()
{
    //TODO: engine mutex here
    if( 0 == mUserEventType ){
        mUserEventType = sdl::RegisterEvents( 1 );

        if( mUserEventType == 0
          || mUserEventType == (Uint32) -1 )     // 0xFFFFFFFF = (Uint32) -1
        {
            pt::log::err << "Could not register custom UserEvent in SDL! Exiting...\n";
            exit(1);
        }
    }
    return mUserEventType;
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
    GetUserEventType(); // first call generates the UserEvent type code
    InitializeConfig();
}


Engine::
~Engine()
{}


void Engine::
SetTickrate( uint32_t rate ) const
{
    assert( false );
}


uint32_t Engine::
GetTickrate() const
{
    assert( false );
}


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
    if( dc != nullptr ){
        dc->DrawScene( t, dt );
    }
}
