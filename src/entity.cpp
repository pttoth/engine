#include "entity.h"

#include "component.h"
#include "services.h"
#include "gamecontrol.h"
#include <assert.h>

#include "alg/std_extension.hpp"

using namespace pttoth::engine;


void Entity::
        RegisterEntity(Entity *subject){
    //register components first
    for(Component* c : subject->_components){
        if( nullptr != c){
        //if component exists
            if( !(c->isRegistered()) ){
            //if it's not registered yet
                Component::RegisterComponent(c);
            }
        }
    }
    //register entity if needed
    if( !subject->isRegistered() ){
        Services::getGameControl()->registerEntity(subject);
    }
    subject->_registered = true;
}

void Entity::
        UnregisterEntity(Entity *subject){
    //if entity's ticking, unreg tick first
    if( subject->isTickRegistered() ){
        Entity::UnregisterTickFunction(subject);
    }

    //unregister entity first
    Services::getGameControl()->unregisterEntity(subject);

    //unregister components
    for(Component* c : subject->_components){
        if( nullptr != c ){
            if(c->isRegistered()){
                Component::UnregisterComponent(c);
            }
        }
    }
    subject->_registered = false;
}

void Entity::
        RegisterTickFunction(Entity *subject, TickGroup group){
    if( subject->isRegistered()
     && !subject->isTickRegistered() ){
        subject->_tick_group = group;
        Services::getGameControl()->registerTick(subject);
        subject->_tick_registered = true;
    }else{
        assert(false); //TODO: throw instead
    }
}

void Entity::
        UnregisterTickFunction(Entity *subject){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        Services::getGameControl()->unregisterTick(subject);
        subject->_tick_registered = false;
    }else{
        assert(false); //TODO: throw instead
    }
}

void Entity::
        AddTickDependency(Entity *subject, Entity *dependency){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        //add dependency registered check
        Services::getGameControl()->addTickDependency(subject, dependency);
    }else{
        assert(false); //TODO: throw instead
    }
}

void Entity::
        RemoveTickDependency(Entity *subject, Entity *dependency){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        Services::getGameControl()->removeTickDependency(subject, dependency);
    }else{
        assert(false); //TODO: throw instead
    }
}

bool Entity::
        isRegistered() const{
    return _registered;
}

bool Entity::
        isTickRegistered() const{
    return _tick_registered;
}

Entity::
        Entity(){
    _tick_enabled = true;
    _tick_group = TickGroup::DURINGPHYSICS;
    _tick_interval = 0.0f;
    _tick_registered = false;
    _tick_last = 0.0f;

    _registered = false;

    _rootComponent = nullptr;

}

Entity::
        Entity(const Entity &other){
}

Entity::
        Entity(Entity &&other){
}

Entity::
        ~Entity(){
}

Entity &Entity::
        operator=(const Entity &other){
}

Entity &Entity::
        operator=(Entity &&other){
}

bool Entity::
        operator==(const Entity &other) const{
}

void Entity::
        addComponent(Component* component){
    int idx = indexOfInVector(_components, component);
    if(idx < 0){
    //if doesn't contain component
        if( isRegistered() ){
        //if entity is registered
            if( !(component->isRegistered()) ){
            //if component is not registered
                //register component
                Component::RegisterComponent(component);
            }
        }
        //add component to array
        Component * comp =  nullptr;
        idx = indexOfInVector(_components, comp);
        if(-1 < idx){
            _components[idx] = component;
        }else{
            _components.push_back(component);
        }
    }
}

void Entity::
        removeComponent(Component *component){
    int idx = indexOfInVector(_components, component);
    if(-1 < idx){
        _components[idx] = nullptr;
    }
}

std::vector<Component*> Entity::
        getComponents(){
    std::vector<Component*> retval;
    retval.reserve( _components.size() );
    for(Component* c : _components){
        if( nullptr != c){
            retval.push_back(c);
        }
    }
    retval.shrink_to_fit();
    return std::move(retval);
}

WorldComponent *Entity::
        getRootComponent(){
    return _rootComponent;
}

void Entity::
        enableTick(){
    _tick_enabled = true;
}

void Entity::
        disableTick(){
    _tick_enabled = false;
}

bool Entity::
        isEnabled() const{
    return _tick_enabled;
}

void Entity::
        tickEntity(float t, float dt){
    float actual_delta;
    if(0.0f == _tick_last){ actual_delta = dt;
    }else{                  actual_delta = t - _tick_last;
    }

    if(_tick_enabled){
        if(_tick_interval <= actual_delta * 1000.0f ){
            for(Component* c : _components){
                if(nullptr != c){
                    c->tick(t, actual_delta);
                }
            }
            tick(t, actual_delta);
            _tick_last = t;
        }
    }
}

void Entity::
        setTickInterval(float interval){
    if(interval < 0.0f){
        throw std::out_of_range("Entity::setTickInterval(): interval value cannot be negative");
    }
    _tick_interval = interval;
}

float Entity::
        getTickInterval() const{
    return _tick_interval;
}

TickGroup Entity::
        getTickGroup() const{
    return _tick_group;
}
