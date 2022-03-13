#include "entity.h"

#include "component.h"
#include "services.h"
#include "enginecontrol.h"
#include <assert.h>

#include "pt/utility.hpp"

using namespace engine;


void Entity::
        RegisterEntity(Entity *subject){
    //register components first
    for(Component* c : subject->mComponents){
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
    subject->mRegistered = true;
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
    for(Component* c : subject->mComponents){
        if( nullptr != c ){
            if(c->isRegistered()){
                Component::UnregisterComponent(c);
            }
        }
    }
    subject->mRegistered = false;
}



void Entity::
        RegisterTickFunction(Entity *subject, TickGroup group){
    if( subject->isRegistered()
     && !subject->isTickRegistered() ){
        subject->mTickGroup = group;
        Services::getGameControl()->registerTick(subject);
        subject->mTickRegistered = true;
    }else{
        assert(false); //TODO: throw instead
    }
}


void Entity::
        UnregisterTickFunction(Entity *subject){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        Services::getGameControl()->unregisterTick(subject);
        subject->mTickRegistered = false;
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
    return mRegistered;
}


bool Entity::
        isTickRegistered() const{
    return mTickRegistered;
}


Entity::
        Entity(){
    mTickEnabled = true;
    mTickGroup = TickGroup::DURINGPHYSICS;
    mTickInterval = 0.0f;
    mTickRegistered = false;
    mTickLast = 0.0f;

    mRegistered = false;

    this->addComponent( &mRootComponent );
}


Entity::
        ~Entity(){

    this->removeComponent( &mRootComponent );
}


bool Entity::
        operator==(const Entity &other) const{
    assert(false); //TODO: implement
    return false;
}


void Entity::
        addComponent(Component* component){
    int idx = pt::IndexOfInVector(mComponents, component);
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
        idx = pt::IndexOfInVector(mComponents, comp);
        if(-1 < idx){
            mComponents[idx] = component;
        }else{
            mComponents.push_back(component);
        }
    }
}


void Entity::
        removeComponent(Component *component){
    int idx = pt::IndexOfInVector(mComponents, component);
    if(-1 < idx){
        mComponents[idx] = nullptr;
    }
}


std::vector<Component*> Entity::
        getComponents(){
    std::vector<Component*> retval;
    retval.reserve( mComponents.size() );
    for(Component* c : mComponents){
        if( nullptr != c){
            retval.push_back(c);
        }
    }
    retval.shrink_to_fit();
    return std::move(retval);
}


const WorldComponent* Entity::
getRootComponent() const
{
    return &mRootComponent;
}

WorldComponent* Entity::
getRootComponent()
{
    return &mRootComponent;
}


void Entity::
Spawn()
{

}

void Entity::
Despawn()
{

}


void Entity::
        enableTick(){
    mTickEnabled = true;
}


void Entity::
        disableTick(){
    mTickEnabled = false;
}


bool Entity::
        isEnabled() const{
    return mTickEnabled;
}


#include <iostream>

void Entity::
        tickEntity(float t, float dt){
    float actual_delta;
    if(0.0f == mTickLast){ actual_delta = dt;
        mTickLast = t - dt;
    }else{                  actual_delta = t - mTickLast;
    }

    if(mTickEnabled){
        if(mTickInterval <= actual_delta * 1000.0f ){
            for(Component* c : mComponents){
                if(nullptr != c){
                    c->tick(t, actual_delta);
                }
            }
            tick(t, actual_delta);
            mTickLast = t;
        }
    }
}


void Entity::
        setTickInterval(float interval){
    if(interval < 0.0f){
        throw std::out_of_range("Entity::setTickInterval(): interval value cannot be negative");
    }
    mTickInterval = interval;
}


float Entity::
        getTickInterval() const{
    return mTickInterval;
}


TickGroup Entity::
        getTickGroup() const{
    return mTickGroup;
}
