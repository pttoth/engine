#include "game.h"

#include <assert.h>
#include "game_events.h"

#include "entity.h"
#include "component.h"

#include "services.h"
#include "gamecontrol.h"

#include <iostream>

#include "SDL2/SDL.h"

using namespace pttoth;
using namespace engine;

Uint32 generate_gametimer_tick(Uint32 interval, void *param){
    SDL_Event ev;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = game_event::EV_GAMETIMER_TICK;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    ev.user = userevent;

    SDL_PushEvent(&ev);
    return(interval);
}



Game::
        Game(): Application(),
                 window(nullptr), renderer(nullptr),
                _uptime(0){
    construct();
}

Game::
        Game(int const argc, char* argv[]):
            Application(argc, argv),
            window(nullptr), renderer(nullptr),
            _uptime{0}{
    construct();
}

void Game::
        construct(){
    _cfg_path= std::string("../../cfg/Engine.cfg");
    initializeConfig();
}

Game::
        ~Game(){
}

void Game::
        onStart(){
    Application::onStart();
    int init = SDL_Init( SDL_INIT_EVENTS
                         | SDL_INIT_TIMER
                         | SDL_INIT_AUDIO
                         | SDL_INIT_VIDEO
                         //| SDL_INIT_HAPTIC
                         | SDL_INIT_JOYSTICK
                         | SDL_INIT_GAMECONTROLLER
                         );
    if( 0 != init  ){
        setErrorMessage("Failed to initialize SDL timer");
    }
    _uptime = SDL_GetTicks();

    atexit(SDL_Quit);

    Services::setGameControl(this);
    Services::setWorld(&world);

    //configure variables
    bool successful_read = readConfig();
    if( !successful_read ){
        std::cout << "warning: could not read config file: "
                  << _cfg_path << std::endl;
        setDefaultSettings();
    }

    Uint32 interval = (Uint32) (1000.0f / _tickrate);
    // there seems to be a problem here ( thread desnyc? )
    //  without the delay, the timer doesn't always start
    SDL_Delay(150);
    _gametimer_id = SDL_AddTimer(interval, generate_gametimer_tick, nullptr);

}

void Game::
        onExit(){


    SDL_RemoveTimer(_gametimer_id);

    GameControl* control = Services::getGameControl();
    if( this == control){
        Services::setGameControl(nullptr);
    }
    Application::onExit();
}

void Game::
        onShutdownSignal(){
    signalShutdownReady();
}

void Game::
        processGameTimerEvent(){
    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - _uptime) / 1000.0f;
    _uptime = current_time;

    //--------------------------------------------------------------------
    //here we catch up to realtime state by processing pending operations
    //  (this can be run parallel to input handling, but has to make sure
    //      to catch up, before proceeding to handling ticks)
    //process Entity/Component registrations and Tick registrations
    processRegistrationsPending();


    //--------------------------------------------------------------------
    //at this point, the engine systems are synced up with the changes

    tickPrePhysics(ft, fdt);
    //2 threads needed here
    //  t1:
    tickDuringPhysics(ft, fdt);
    //  t2:
//TODO: world.updatePhysics();

    tickPostPhysics(ft, fdt);
    tick(ft,fdt);

    // this will happen on a different thread, won't access anything here
    //drawScene(ft, fdt);
}

void Game::
        registerEntity(Entity *e){
    PendingTask ptr(e, PendingTask::Task::REGISTER_ENTITY);
    _pending_tasks.push_back(ptr);
}

void Game::
        unregisterEntity(Entity *e){
    PendingTask ptr(e, PendingTask::Task::UNREGISTER_ENTITY);
    _pending_tasks.push_back(ptr);
}

void Game::
        registerComponent(Component *c){
    PendingTask ptr(c, PendingTask::Task::REGISTER_COMPONENT);
    _pending_tasks.push_back(ptr);
}

void Game::
        unregisterComponent(Component *c){
    PendingTask ptr(c, PendingTask::Task::UNREGISTER_COMPONENT);
    _pending_tasks.push_back(ptr);
}

void Game::
        onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void Game::
        onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void Game::
        onMouseMotion(int32_t x, int32_t y,
                      int32_t x_rel, int32_t y_rel,
                      uint32_t timestamp, uint32_t mouseid){
}

void Game::
        onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction){
}

void Game::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
}

void Game::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
}

void Game::
        onTouchInputEvent(){
    assert(false);
}

void Game::
        initializeConfig(){
    _cfg.setPath(_cfg_path);
    cfgAddKey(_cfg, iTickRate);
}

void Game::
        setDefaultSettings(){
    _tickrate = 50;
    _cfg.setI(iTickRate, _tickrate);
}

bool Game::
        readConfig(){
    try{
        _cfg.read();
        int tickrate = _cfg.getI(iTickRate);

        //by now, all reads were successful,
        //  we can start setting the variables
        _tickrate = tickrate;

    }catch(...){
        return false;
    }
    return true;
}

void Game::
        onEvent(SDL_Event* event){
    SDL_Event ev = *event; //avoid unneccessary repeat of dereferences
    switch(ev.type){
    case SDL_MOUSEMOTION:
        if(! (ev.motion.which & SDL_TOUCH_MOUSEID) ){ //if not touchpad event
            onMouseMotion(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel, ev.motion.timestamp, ev.motion.which);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown(ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which);
        break;
    case SDL_MOUSEBUTTONUP:
        onMouseButtonUp(ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which);
        break;
    case SDL_MOUSEWHEEL:
        onMouseWheel(ev.wheel.x, ev.wheel.y, ev.wheel.timestamp, ev.wheel.which, ev.wheel.direction);
        break;
    case SDL_KEYDOWN:
        onKeyDown(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat);
        break;
    case SDL_KEYUP:
        onKeyUp(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat);
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
        case game_event::EV_GAMETIMER_TICK:
            processGameTimerEvent();
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

#include "game.h"

#include <assert.h>
#include "entity.h"

using namespace pttoth;
using namespace engine;

void Game::
        registerTick(Entity *e){
    PendingTask ptr(e,
                    e->getTickGroup(),
                    PendingTask::Task::REGISTER_TICK);
    _pending_tasks.push_back(ptr);
}

void Game::
        unregisterTick(Entity *e){
    PendingTask ptr(e,
                    e->getTickGroup(),
                    PendingTask::Task::UNREGISTER_TICK);
    _pending_tasks.push_back(ptr);
}

void Game::
        addTickDependency(Entity *subject, Entity *dependency){
    PendingTask ptr(subject,
                    subject->getTickGroup(),
                    PendingTask::Task::REGISTER_TICK_DEPENDENCY,
                    dependency);
    _pending_tasks.push_back(ptr);
}

void Game::
        removeTickDependency(Entity *subject, Entity *dependency){
    PendingTask ptr(subject,
                    subject->getTickGroup(),
                    PendingTask::Task::UNREGISTER_TICK_DEPENDENCY,
                    dependency);
    _pending_tasks.push_back(ptr);
}

void Game::
        removeEntityDependencies(Entity *subject){
    PendingTask ptr(subject,
                       subject->getTickGroup(),
                       PendingTask::Task::REMOVE_ENTITY_DEPENDENCIES);
    _pending_tasks.push_back(ptr);
}

void Game::
        removeDependenciesReferencingEntity(Entity *dependency){
    PendingTask ptr(dependency,
                       dependency->getTickGroup(),
                       PendingTask::Task::REMOVE_DEPENDENCIES_REFERENCING_ENTITY);
    _pending_tasks.push_back(ptr);
}

std::vector<Game::TickDependencyData> &Game::
        getTickGroupContainer(TickGroup tg){
    assert( tg != TickGroup::NO_GROUP);
    switch( tg ){
    case TickGroup::PREPHYSICS:     return _tick_prephysics;
    case TickGroup::DURINGPHYSICS:  return _tick_duringphysics;
    case TickGroup::POSTPHYSICS:    return _tick_postphysics;
    default:                        assert(false); //mem garbage value
    }
}

void Game::
        processEntityRegister(Entity *subject){
    //make sure subject is not present
    int idx = indexOfInVector(_entities, subject);
    assert(idx < 0);

    _entities.push_back(subject);
    subject->OnRegister();
}

void Game::
        processEntityUnregister(Entity *subject){
    //make sure subject is present
    int idx = indexOfInVector(_entities, subject);
    assert(-1 < idx);

    removeElementInVector(_entities, idx);
    subject->OnUnregister();
}

void Game::
        processComponentRegister(Component *subject){
    //make sure subject is not present
    int idx = indexOfInVector(_components, subject);
    assert(idx < 0);

    _components.push_back(subject);
    subject->OnRegistered();
}

void Game::
        processComponentUnregister(Component *subject){
    //make sure subject is present
    int idx = indexOfInVector(_components, subject);
    assert(-1 < idx);

    removeElementInVector(_components, idx);
    subject->OnUnregistered();
}

void Game::
        processRegistrationsPending(){
    for(PendingTask ptr : _pending_tasks){
        switch(ptr.task){
        case PendingTask::Task::REGISTER_ENTITY:
            processEntityRegister(ptr.subject);
            break;
        case PendingTask::Task::UNREGISTER_ENTITY:
            processEntityUnregister(ptr.subject);
            break;
        case PendingTask::Task::REGISTER_COMPONENT:
            processComponentRegister(ptr.subject_component);
            break;
        case PendingTask::Task::UNREGISTER_COMPONENT:
            processComponentUnregister(ptr.subject_component);
            break;
        case PendingTask::Task::REGISTER_TICK:
            processTickRegister(ptr.subject, ptr.group);
            break;
        case PendingTask::Task::UNREGISTER_TICK:
            processTickUnregister(ptr.subject, ptr.group);
            break;
        case PendingTask::Task::REGISTER_TICK_DEPENDENCY:
            processTickDependencyRegister(ptr.subject, ptr.dependency);
            break;
        case PendingTask::Task::UNREGISTER_TICK_DEPENDENCY:
            processTickDependencyUnregister(ptr.subject, ptr.dependency);
            break;
        case PendingTask::Task::REMOVE_ENTITY_DEPENDENCIES:
            processTickDependencyRemoveAll(ptr.subject);
            break;
        case PendingTask::Task::REMOVE_DEPENDENCIES_REFERENCING_ENTITY:
            processTickDependencyReferenceCleanup(ptr.subject);
            break;
        default:
            assert(false);
            break;
        }
    }
    _pending_tasks.clear();
}

void Game::
        processTickRegister(Entity* subject, TickGroup group){
    TickDependencyData id(subject);
    std::vector<TickDependencyData>& vec_tickgroup =
            getTickGroupContainer(group);

    //check if subject is already present
    int idx = indexOfInVector(vec_tickgroup, id);
    assert(idx < 0);

    vec_tickgroup.push_back(id); //add
}

void Game::
        processTickUnregister(Entity* subject, TickGroup group){
    TickDependencyData id(subject);
    std::vector<TickDependencyData>& vec_tickgroup =
            getTickGroupContainer(group);

    //check if subject is missing
    int idx = indexOfInVector(vec_tickgroup, id);
    assert(-1 < idx);

    removeElementInVector(vec_tickgroup, idx); //remove
}

void Game::
        processTickDependencyRegister(Entity *subject, Entity *dependency){
    TickDependencyData id_subject(subject);
    TickDependencyData id_dependency(dependency);
    std::vector<TickDependencyData>& vec_tickgroup =
            getTickGroupContainer( subject->getTickGroup() );

    //make sure, that dependency is in the same tick group
        //this may happen during runtime, so
        //TODO: report ERROR with notification service (when it's done)
    assert( 0 <= indexOfInVector(vec_tickgroup, id_dependency) );

    //make sure subject is present in the group
    int idx = indexOfInVector(vec_tickgroup, id_subject);
    assert(-1 < idx);

    //check if it already holds the dependency
    int idx_dep = indexOfInVector(vec_tickgroup[idx].dependencies, dependency);
    if(idx_dep < 0){ //if dependency is not contained yet
        vec_tickgroup[idx].dependencies.push_back(dependency); //add dependency
    }
}

void Game::
        processTickDependencyUnregister(Entity* subject, Entity* dependency){
    TickDependencyData id_subject(subject);
    std::vector<TickDependencyData>& vec_tickgroup =
            getTickGroupContainer(subject->getTickGroup());

    //make sure subject is present in the group
    int idx = indexOfInVector(vec_tickgroup, id_subject);
    assert(-1 < idx);

    //check if it contains the dependency
    int idx_dep = indexOfInVector(vec_tickgroup[idx].dependencies, dependency);
    if(-1 < idx_dep){
        removeElementInVector(vec_tickgroup[idx].dependencies, idx_dep);
    }
}

void Game::
        processTickDependencyRemoveAll(Entity *subject){
    TickDependencyData id_subject(subject);
    std::vector<TickDependencyData>& vec_tickgroup =
                getTickGroupContainer(subject->getTickGroup());

    //make sure subject is present in the group
    int idx = indexOfInVector(vec_tickgroup, id_subject);
    assert(-1 < idx);

    //remove all dependencies
    vec_tickgroup[idx].dependencies.clear();
}

void Game::
        processTickDependencyReferenceCleanup(Entity *dependency){
    std::vector<TickDependencyData>& vec_tickgroup =
                getTickGroupContainer(dependency->getTickGroup());

    //for each ticker
    for(auto tdd : vec_tickgroup){
        //check if it depends on 'dependency'
        int idx_dep = indexOfInVector(tdd.dependencies, dependency);
        if(-1 < idx_dep){
            //remove dependency
            removeElementInVector(tdd.dependencies, idx_dep);
        }
    }
}

void Game::
        clearUnusedTickData(){
    TickGroup groups[] ={TickGroup::PREPHYSICS,
                         TickGroup::DURINGPHYSICS,
                         TickGroup::POSTPHYSICS};
    for(auto tg : groups){
        std::vector<TickDependencyData>& vec_tickgroup =
                getTickGroupContainer( tg );
        //iterate backwards (removal messes up right side of vector)
        for(int idx=vec_tickgroup.size(); 0<idx; --idx){
            //remove each inactive entry (starting from backwards)
            if(!vec_tickgroup[idx].active){
                removeElementInVector(vec_tickgroup, idx);
            }
        }
    }
}

void Game::
        tickThisGroupContainer(std::vector<TickDependencyData> &tg_container,
                               float t, float dt){
    size_t size = tg_container.size();
    size_t count = 0;
    size_t safety = 0;
    //don't tick empty container
    if(0 == size){
        return;
    }
    //reset container metadata
    for(TickDependencyData& d : tg_container){
        d.ticked = false; //doesnt care about active/inactive check (no need and more costly to check)
    }
    while(count < size){
        //ran 'size' times and couldn't tick everyone!
        assert(safety < size);

        for(TickDependencyData& tdd : tg_container){
            if( tdd.shouldTick() ){
                //resolve dependencies
                bool canGo = true;
                for(Entity* dep : tdd.dependencies){
                    //find dependency
                    TickDependencyData d(dep);
                    int idx_dep = indexOfInVector( tg_container , d);
                    if( !tg_container[idx_dep].ticked ){
                    //if dependency haven't ticked, skip us for now
                        canGo = false;
                    }
                }
                //if dependencies are done, tick entity
                //  otherwise wait for next pass
                if(canGo){
                    tdd.entity->tickEntity(t, dt);
                    tdd.ticked = true;
                    ++count;
                }
            }
        }
        ++safety;
    }
}

void Game::
        tickPrePhysics(float t, float dt){
    tickThisGroupContainer(_tick_prephysics, t, dt);
}

void Game::
        tickDuringPhysics(float t, float dt){
    tickThisGroupContainer(_tick_duringphysics, t, dt);
}

void Game::
        tickPostPhysics(float t, float dt){
    tickThisGroupContainer(_tick_postphysics, t, dt);
}

//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------


void Game::
        drawScene(float t, float dt){

}

