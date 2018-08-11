#include "game.h"

#include <assert.h>
#include "game_events.h"

#include "entity.h"
#include "component.h"

#include "services.h"

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
}

Game::
        Game(int const argc, char* argv[]):
            Application(argc, argv),
            window(nullptr), renderer(nullptr),
            _uptime{0}{
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
    if(control = (GameContol*)this){
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

//  here we commit any tick registrations
//    that happened since the start of the last frame calculation
    processTickRegistrationsPending();

    tickPrePhysics(ft, fdt);
    //2 threads needed here
    //  t1:
    tickDuringPhysics(ft, fdt);
    //  t2:
//TODO: world.updatePhysics();

    tickPostPhysics(ft, fdt);
    drawScene(ft, fdt);
}

void Game::
        registerEntity(Entity *e){
    if( !(_entities.contains(e)) ){
        _entities.push_back(e);
        e->OnRegister();
    }
}

void Game::
        unregisterEntity(Entity *e){
    if( _entities.contains(e) ){
        _entities.remove(e);
        e->OnUnregister();
    }
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
    PendingTickReg ptr(e,
                       e->getTickGroup(),
                       PendingTickReg::Task::REGISTER_TICK
                       );
    _tick_reg_pending.push_back(ptr);
}

void Game::
        unregisterTick(Entity *e){
    PendingTickReg ptr(e,
                       e->getTickGroup(),
                       PendingTickReg::Task::UNREGISTER_TICK
                       );
    removeAllDependencies(e);
}

void Game::
        addTickDependency(Entity *subject, Entity *dependency){
    PendingTickReg ptr(subject,
                       subject->getTickGroup(),
                       PendingTickReg::Task::REGISTER_TICK_DEPENDENCY,
                       dependency);
    _tick_reg_pending.push_back(ptr);
}

void Game::
        removeTickDependency(Entity *subject, Entity *dependency){
    PendingTickReg ptr(subject,
                       subject->getTickGroup(),
                       PendingTickReg::Task::UNREGISTER_TICK_DEPENDENCY,
                       dependency);
    _tick_reg_pending.push_back(ptr);
}

void Game::
        removeAllDependencies(Entity *e){
    PendingTickReg ptr(subject,
                       subject->getTickGroup(),
                       PendingTickReg::Task::REMOVE_ENTITY_DEPENDENCIES,
                       );
    _tick_reg_pending.push_back(ptr);
}

std::vector<Game::TickDependencyData *> &Game::
        getTickGroupContainer(TickGroup tg){
    switch( tg ){
    case TickGroup::PREPHYSICS:     return _tick_prephysics;
    case TickGroup::DURINGPHYSICS:  return _tick_duringphysics;
    case TickGroup::POSTPHYSICS:    return _tick_postphysics;
    default: assert(false);
    }
}

void Game::
        processTickRegistrationsPending(){
    for(PendingTickReg ptr : _tick_reg_pending){
        switch(ptr.task){
        case PendingTickReg::Task::REGISTER_TICK:
            processTickRegister(ptr.entity, ptr.group);
            break;
        case PendingTickReg::Task::UNREGISTER_TICK:
            processTickUnregister(ptr.entity, ptr.group);
            break;
        case PendingTickReg::Task::REGISTER_TICK_DEPENDENCY:
            processTickDependencyRegister(ptr.entity, ptr.param);
            break;
        case PendingTickReg::Task::UNREGISTER_TICK_DEPENDENCY:
            processTickDependencyUnregister(ptr.entity, ptr.param);
            break;
        case PendingTickReg::Task::REMOVE_ENTITY_DEPENDENCIES:
            processTickDependencyCleanup(ptr.entity);
            break;
        }
    }
    _tick_reg_pending.clear();
}

void Game::
        processTickRegister(Entity* subject, TickGroup group){
    TickDependencyData id(subject);
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(group);

    //find subject data
    int idx = indexOfInVector(vec_tickgroup, id);
    if( idx < 0){ //if doesn't exist
        vec_tickgroup.push_back(id); //add
    }else{ //if exists
        if(bKeepUnusedDependencies){
            if( !(vec_tickgroup[idx].active) ){
                vec_tickgroup[idx].active = true; //re-activate
            }
        }
    }
}

void Game::
        processTickUnregister(Entity* subject, TickGroup group){
    TickDependencyData id(subject);
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(group);

    //find subject data
    int idx = indexOfInVector(vec_tickgroup, id);
    if( -1 < idx){ //if it exists
        if(bKeepUnusedDependencies){
            vec_tickgroup[idx].active = false;
        }else{
            removeElementInVector(vec_tickgroup, idx);
        }
    }
}

void Game::
        processTickDependencyRegister(Entity *subject, Entity *dependency){
    TickDependencyData id_subject(subject);
    TickDependencyData id_dependency(dependency);
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(subject->getTickGroup());
    //make sure, that dependency is in the same tick group
    if( -1 < indexOfInVector(vec_tickgroup, id_dependency) ){
        //find subject data
        int idx = indexOfInVector(vec_tickgroup, id_subject);
        if(idx < 0){ //if its not contained
            if(bKeepUnusedDependencies){
                id_subject.active = false; //we add only the dependency, not the tick reg
                vec_tickgroup.push_back(id_subject);
                idx = vec_tickgroup.size();
            }else{
                assert(false); // subject not contained in tickgroup
                //TODO: should throw this instead
                return;
            }
        }
        //---past here, it's surely contained under idx---
        //check if it already holds the dependency
        idx_dep = indexOfInVector(vec_tickgroup[idx].dependencies,
                                  dependency);
        if(idx_dep < 0){ //if dependency not contained yet
            vec_tickgroup[idx].dependencies.push_back(dependency); //add dependency
        }
    }else{
        assert(false); //dependency not contained in the tickgroup
        //TODO: should throw this instead
    }
}

void Game::
        processTickDependencyUnregister(Entity* subject, Entity* dependency){
    TickDependencyData id_subject(subject);
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(subject->getTickGroup());

    int idx = indexOfInVector(vec_tickgroup, id_subject);
    if(-1 < idx){
        int idx_dep = indexOfInVector(vec_tickgroup[idx].dependencies, dependency);
        if(-1 < idx_dep){
            removeElementInVector(vec_tickgroup[idx].dependencies, dependency);
        }
    }
}

void Game::
        processTickDependencyCleanup(Entity *dependecy){
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(dependecy->getTickGroup());

    //for each ticker
    for(auto tdd : vec_tickgroup){
        int idx_dep = indexOfInVector(tdd.dependencies, dependecy);
        if(-1 < idx_dep){
        //if it depends on subject
            //remove dependency
            removeElementInVector(tdd.dependencies, idx_dep);
        }
    }
}

void Game::
        clearUnusedTickData(){
    TickDependencyData id_subject(subject);
    std::vector<TickDependencyData>& vec_tickgroup;
    vec_tickgroup = getTickGroupContainer(subject->getTickGroup());

    //iterate backwards (removal messes up right side of vector)
    for(int idx=vec_tickgroup.size(); 0<idx; --idx){
        //remove each inactive entry (starting from backwards)
        if(!vec_tickgroup[idx].active){
            removeElementInVector(vec_tickgroup, id_subject);
        }
    }
}

void Game::
        tickThisGroupContainer(std::vector<TickDependencyData> &tg_container,
                               float t, float dt){
    size_t size = tg_container.size();
    size_t count = 0;
    size_t safety = 0;
    //reset container metadata
    for(TickDependencyData d : tg_container){
        d.ticked = false; //doesnt care about active/inactive check (no need and more costly to check)
    }
    while(count < size){
        //ran 'size' times and couldn't tick everyone!
        assert(safety < size);

        for(TickDependencyData tdd : tg_container){
            if( tdd.shouldTick() ){
                //resolve dependencies
                bool canGo = true;
                for(TickDependencyData* d : tdd.dependencies){
                    //if dependency haven't ticked, skip us for now
                    if( d->shouldTick() ){
                        canGo = false;
                        break;
                    }
                }
                //if dependencies are done, tick entity
                //else  wait for next pass
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

