#include "entity.h"

#include "component.h"
#include "services.h"
#include "gamecontrol.h"
#include <assert.h>

using namespace pttoth::engine;

int Entity::
        indexOfComponent(Component* const c) const{
    for(int i=0; i<_components.size(); ++i ){
        if( c == _components[i]){
            return i;
        }
    }
    return -1;
}


void Entity::
        RegisterEntity(Entity *e){
    //register components first
    for(Component* c : _components){
        if( nullptr != c){
            Component::RegisterComponent(c);
        }
    }
    //register entity
    Services::getGameControl()->registerEntity(e);
}

void Entity::
        UnregisterEntity(Entity *e){
    //unregister entity first
    Services::getGameControl()->unregisterEntity(e);
    //unregister components
    for(Component* c : _components){
        if( nullptr != c){
            Component::UnregisterComponent(c);
        }
    }
}

void Entity::
        RegisterTickFunction(Entity *e){
    if(!_tick_registered){
        Services::getGameControl()->registerTick(e);
        _tick_registered = true;
    }
}

void Entity::
        UnregisterTickFunction(Entity *e){
    if(_tick_registered){
        Services::getGameControl()->unregisterTick(e);
        _tick_registered = false;
    }
}

void Entity::
        AddTickDependency(Entity *subject, Entity *dependency){
    if(_tick_registered){
        Services::getGameControl()->addTickDependency(subject, dependency);
    }
}

void Entity::
        RemoveTickDependency(Entity *subject, Entity *dependency){
    if(_tick_registered){
        Services::getGameControl()->removeTickDependency(subject, dependency);
    }
}

bool Entity::
        isRegistered(){
    return _entity_registered;
}

Entity::
        Entity(){
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
    size_t idx = indexOfComponent(component);
    if(idx < 0){
    //if contains component
        if( isRegistered() ){
        //if entity is registered
            if( !(component->isRegistered()) ){
            //if component is not registered
                //register component
                Component::RegisterComponent(component);
            }
        }
        //add component to array
        int idx = indexOfComponent(nullptr);
        if(-1 < idx){
            _components[i] = component;
        }else{
            _components.push_back(component);
        }
    }
}

void Entity::
        removeComponent(Component *component){
    size_t idx = indexOfComponent(component);
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
        isEnabled(){
    return _tick_enabled;
}

void Entity::
        addTickDependency(Entity *e){
    if(_tick_registered){
        Services.getGameControl()->addTickDependency(this, e);
        _tick_registered;
    }else{
        assert(false); //throw instead
    }
}

void Entity::
        RemoveTickDependency(Entity* e){
    if(_tick_registered){
        Services.getGameControl()->removeTickDependency(this, e);
    }else{
        assert(false); //throw instead
    }
}

void Entity::
        tickEntity(float t, float dt){
    if(_tick_enabled){
        if(dt <= t-_last_tick ){
            for(Component* c : _components){
                if(nullptr != c){
                    c->tick(t, dt);
                }
            }
            tick(t, dt);
            _last_tick = t;
        }
    }
}

void Entity::
        setTickInterval(float interval){
    if(0.0f < interval){    _tick_interval = interval;}
    else{                   _tick_interval = 0.0f;    }
}

void Entity::
        setTickGroup(TickGroup tg){
    Services.getGameControl()->unregisterTick(this);
    _tick_group = tg;
    Services.getGameControl()->registerTick(this);
}

TickGroup Entity::
        getTickGroup() const{
    return _tick_group;
}
