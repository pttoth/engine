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
Engine(): SDLApplication(),
          mWindow(nullptr), mRenderer(nullptr),
          mUptime(0), mGametimerId(0),
          mPendingTasksTrigger(), mPendingTasks(mPendingTasksTrigger)
{
    Construct();
}


Engine::
Engine(int const argc, char* argv[]):
       SDLApplication(argc, argv),
       mWindow(nullptr), mRenderer(nullptr),
       mUptime{0}, mGametimerId(0),
       mPendingTasksTrigger(), mPendingTasks(mPendingTasksTrigger)
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

    Services::SetEngineControl( this );
    Services::SetDrawingControl( &mDrawingManager );

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

    World* w = Services::GetWorld();
    if( &mWorld == w ){
        Services::SetWorld( nullptr );
    }

    DrawingControl* dc = Services::GetDrawingControl();
    if( &mDrawingManager == dc){
        Services::SetDrawingControl( nullptr );
    }

    EngineControl* control = Services::GetEngineControl();
    if( this == control ){
        Services::SetEngineControl( nullptr );
    }

    SDLApplication::OnExit();
}


void Engine::
OnShutdownSignal()
{
    signalShutdownReady();
}


void Engine::
ProcessGameTimerEvent()
{
    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - mUptime) / 1000.0f;
    mUptime = current_time;

    //--------------------------------------------------------------------
    //here we catch up to realtime state by processing pending operations
    //  (this can be run parallel to input handling, but has to make sure
    //      to catch up, before proceeding to handling ticks)
    //process Entity/Component registrations and Tick registrations
    ProcessRegistrationsPending();


    //--------------------------------------------------------------------
    //at this point, the engine systems are synced up with the changes

    TickPrePhysics( ft, fdt );
    //2 threads needed here
    //  t1:
    TickDuringPhysics( ft, fdt );
    //  t2:
//TODO: world.updatePhysics();

    TickPostPhysics( ft, fdt );
    Tick( ft,fdt );

    // this will happen on a different thread, won't access anything here
    drawScene( ft, fdt );
}


void Engine::
RegisterEntity(Entity& entity)
{
    Entity* pe = &entity; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        //make sure subject is not present
        int idx = pt::IndexOfInVector( mEntities, pe );
        assert( idx < 0 );

        mEntities.push_back( pe );
        pe->OnRegister();

    }, EventExecRule::TriggerOnce );
}


void Engine::
UnregisterEntity(Entity& entity)
{
    Entity* pe = &entity; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        //make sure subject is present
        int idx = pt::IndexOfInVector( mEntities, pe );
        assert( -1 < idx );

        pt::RemoveElementInVector( mEntities, idx );
        pe->OnUnregister();

    }, EventExecRule::TriggerOnce );
}


void Engine::
RegisterComponent(Component& component)
{
    Component* pc = &component; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        //make sure subject is not present
        int idx = pt::IndexOfInVector( mComponents, pc );
        assert( idx < 0 );

        mComponents.push_back( pc );
        pc->OnRegistered();

    }, EventExecRule::TriggerOnce );
}


void Engine::
UnregisterComponent(Component& component)
{
    Component* pc = &component; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        //make sure subject is present
        int idx = pt::IndexOfInVector( mComponents, pc );
        assert( -1 < idx );

        pt::RemoveElementInVector( mComponents, idx );
        pc->OnUnregistered();

    }, EventExecRule::TriggerOnce );
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
//                  Tick Handling
//--------------------------------------------------
//--------------------------------------------------

using namespace pt;
using namespace engine;

void Engine::
RegisterTick(Ticker& subject)
{
    Ticker* ps = &subject; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id( ps );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( ps->GetTickGroup() );

        //check if subject is already present
        int idx = pt::IndexOfInVector( vec_tickgroup, id );
        assert( idx < 0 );

        vec_tickgroup.push_back( id );

    }, EventExecRule::TriggerOnce );
}


void Engine::
UnregisterTick(Ticker &subject)
{
    Ticker* ps = &subject; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id( ps );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( ps->GetTickGroup() );

        //check if subject is missing
        int idx = pt::IndexOfInVector( vec_tickgroup, id );
        assert( -1 < idx );

        pt::RemoveElementInVector( vec_tickgroup, idx );

    }, EventExecRule::TriggerOnce );
}


void Engine::
AddTickDependency(Ticker& subject, Ticker& dependency)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer
    Ticker* pdep = &dependency;

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        TickDependencyData id_dependency( pdep );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure, that dependency is in the same tick group
            //this may happen during runtime, so
            //TODO: report ERROR with notification service (when it's done)
        assert( 0 <= pt::IndexOfInVector( vec_tickgroup, id_dependency ) );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //check if it already holds the dependency
        int idx_dep = pt::IndexOfInVector( vec_tickgroup[idx].dependencies, pdep );
        if( idx_dep < 0 ){ //if dependency is not contained yet
            vec_tickgroup[idx].dependencies.push_back( pdep );
        }

    }, EventExecRule::TriggerOnce );
}


void Engine::
RemoveTickDependency(Ticker& subject, Ticker& dependency)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer
    Ticker* pdep = &dependency;

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //check if it contains the dependency
        int idx_dep = pt::IndexOfInVector( vec_tickgroup[idx].dependencies, pdep );
        if( -1 < idx_dep ){
            pt::RemoveElementInVector( vec_tickgroup[idx].dependencies, idx_dep );
        }

    }, EventExecRule::TriggerOnce );
}


void Engine::
RemoveEntityDependencies(Ticker& subject)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //remove all dependencies
        vec_tickgroup[idx].dependencies.clear();

    }, EventExecRule::TriggerOnce );
}


void Engine::
RemoveDependenciesReferencingEntity(Ticker& dependency)
{
    Ticker* pdep = &dependency; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( pdep->GetTickGroup() );

        //for each ticker
        for( auto tdd : vec_tickgroup ){
            //check if it depends on 'dependency'
            int idx_dep = pt::IndexOfInVector( tdd.dependencies, pdep );
            if( -1 < idx_dep ){
                //remove dependency
                pt::RemoveElementInVector( tdd.dependencies, idx_dep );
            }
        }

    }, EventExecRule::TriggerOnce );
}


std::vector<Engine::TickDependencyData> &Engine::
GetTickGroupContainer(Ticker::Group tg)
{
    assert( tg != Ticker::Group::NO_GROUP);
    switch( tg ){
    case Ticker::Group::PREPHYSICS:     return mTickDepPrephysics;
    case Ticker::Group::DURINGPHYSICS:  return mTickDepDuringphysics;
    case Ticker::Group::POSTPHYSICS:    return mTickDepPostphysics;
    default:                            assert(false); //mem garbage value
    }
}


void Engine::
ProcessRegistrationsPending()
{
    mPendingTasksTrigger();
    mPendingTasks.clear();
}


void Engine::
ClearUnusedTickData()
{
    Ticker::Group groups[] ={Ticker::Group::PREPHYSICS,
                             Ticker::Group::DURINGPHYSICS,
                             Ticker::Group::POSTPHYSICS };
    for(auto tg : groups){
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( tg );
        //iterate backwards (removal messes up right side of vector)
        for( int idx=vec_tickgroup.size(); 0<=idx; --idx ){
            //remove each inactive entry (starting from backwards)
            if( !vec_tickgroup[idx].active ){
                pt::RemoveElementInVector( vec_tickgroup, idx );
            }
        }
    }
}


void Engine::
TickElementsInGroupContainer(std::vector<TickDependencyData> &tg_container,
                             float t, float dt)
{
    size_t size = tg_container.size();
    size_t count = 0;
    size_t safety = 0;
    //don't tick empty container
    if( 0 == size ){
        return;
    }
    //reset container metadata
    for( TickDependencyData& d : tg_container ){
        d.ticked = false; //doesnt care about active/inactive check (no need and more costly to check)
    }
    while( count < size ){
        //ran 'size' times and couldn't tick everyone!
        assert( safety < size );

        for(TickDependencyData& tdd : tg_container){
            if( tdd.shouldTick() ){
                //resolve dependencies
                bool canGo = true;
                for( Ticker* dep : tdd.dependencies ){
                    //find dependency
                    TickDependencyData d(dep);
                    int idx_dep = pt::IndexOfInVector( tg_container , d);
                    if( !tg_container[idx_dep].ticked ){
                    //if dependency haven't ticked, skip us for now
                        canGo = false;
                    }
                }
                //if dependencies are done, tick entity
                //  otherwise wait for next pass
                if( canGo ){
                    tdd.subject->Tick( t, dt );
                    tdd.ticked = true;
                    ++count;
                }
            }
        }
        ++safety;
    }
}


void Engine::
TickPrePhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepPrephysics, t, dt );
}


void Engine::
TickDuringPhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepDuringphysics, t, dt );
}


void Engine::
TickPostPhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepPostphysics, t, dt );
}


//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------


void Engine::
drawScene(float t, float dt)
{}

