#include "engine/Engine.h"

#include "engine/gl/GlWrapper.h"
#include "engine/CameraPerspective.h"
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


bool Engine::
DeveloperMode() const
{
    return mDeveloperMode;
}


void Engine::
DeveloperMode( bool value )
{
    mDeveloperMode = value;
}


void Engine::
Execute()
{
    assert( not mMainLoopActive );
    if( mMainLoopActive ){
        return;
    }

    OnStart();
    mMainLoopActive = true;

    while( mMainLoopActive ){
        Update();
    }
    OnExit();
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
        const std::string errormsg( "Failed to initialize SDL" );
        this->SetErrorMessage( errormsg.c_str() );

        throw std::runtime_error( errormsg.c_str() );
    }
    mUptime = SDL_GetTicks();

    atexit( SDL_Quit );

    // GL context is created here
    mDrawingManager.Initialize();

    Services::SetDrawingControl( &mDrawingManager );
    Services::SetScheduler( &mScheduler );

    Services::SetWorld( &mWorld );

    mCamera = NewPtr<CameraPerspective>( "MainCamera" );
    Services::GetDrawingControl()->SetMainCamera( mCamera );


    //configure variables
    bool successful_read = ReadConfig();
    if( !successful_read ){
        std::cout << "warning: could not read config file: "
                  << mCfgPath << std::endl;
        SetDefaultSettings();
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
}


void Engine::
SetDefaultSettings()
{}


bool Engine::
ReadConfig()
{
    try{
        //mCfg.read();

    }catch(...){
        return false;
    }
    return true;
}


void Engine::
Update()
{
    SDL_Event ev;
    // handle new events/inputs/etc.
    while( SDL_PollEvent( &ev ) ){
        OnEvent( &ev );
    }

    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - mUptime) / 1000.0f;
    mUptime = current_time;

    // Actor Tick+TickDependency [un]registrations get executed here
    mScheduler.ProcessPendingTasks();

    // Tick all Actors
    mScheduler.TickPrePhysics( ft, fdt );
    mScheduler.TickDuringPhysics( ft, fdt );
    mScheduler.TickPostPhysics( ft, fdt );

    UpdateGameState( ft, fdt );

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
        default:
            break;
        }
        break;
    case SDL_QUIT:
        mMainLoopActive = false;
        //Quit();
        break;
    }
}


//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------


void Engine::
drawScene( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    if( dc != nullptr ){
        dc->DrawScene( t, dt );
    }
}
