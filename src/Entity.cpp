#include "Entity.h"

#include "Component.h"
#include "Services.h"
#include "EngineControl.h"
#include <assert.h>

#include <sstream>

#include "pt/utility.hpp"

#include "pt/logging.h"

using namespace engine;


std::string
GenerateRootComponentName(const std::string& entityname)
{
    std::stringstream ss;
    ss << entityname << ".Root";
    return ss.str();
}


entity::ComponentVisitor::
ComponentVisitor(Entity& entity, Component& component):
    mEntity(&entity),
    mComponent(&component)
{}


void entity::ComponentVisitor::
AddWorldComponent()
{
    WorldComponent* wc = dynamic_cast<WorldComponent*>(mComponent);
    assert(wc);
    if(!wc){
        pt::log::err << "Entity::ComponentVisitor: Invalid component conversion in Entity '" << mEntity->GetName() << "'\n";
    }else{
        mEntity->AddWorldComponent( wc );
    }
}


void entity::ComponentVisitor::
RemoveWorldComponent()
{
    WorldComponent* wc = dynamic_cast<WorldComponent*>(mComponent);
    assert(wc);
    if(!wc){
        pt::log::err << "Entity::ComponentVisitor: Invalid component conversion in Entity '" << mEntity->GetName() << "'\n";
    }else{
        mEntity->RemoveWorldComponent( wc );
    }
}



const std::string& Entity::
GetName() const
{
    return mName;
}


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
        Services::getEngineControl()->registerEntity(subject);
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
    Services::getEngineControl()->unregisterEntity(subject);

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
        Services::getEngineControl()->registerTick(subject);
        subject->mTickRegistered = true;
    }else{
        assert(false); //TODO: throw instead
    }
}


void Entity::
        UnregisterTickFunction(Entity *subject){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        Services::getEngineControl()->unregisterTick(subject);
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
        Services::getEngineControl()->addTickDependency(subject, dependency);
    }else{
        assert(false); //TODO: throw instead
    }
}


void Entity::
        RemoveTickDependency(Entity *subject, Entity *dependency){
    if( subject->isRegistered()
     && subject->isTickRegistered() ){
        Services::getEngineControl()->removeTickDependency(subject, dependency);
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
Entity(const std::string& name):
    mName(name),
    mRootComponent( GenerateRootComponentName( this->GetName() ) )

{
    //TODO: check which of these should be const
    mTickEnabled = true;
    mTickGroup = TickGroup::DURINGPHYSICS;
    mTickInterval = 0.0f;
    mTickRegistered = false;
    mTickLast = 0.0f;

    mRegistered = false;

    this->addComponent( &mRootComponent );
}


Entity::
Entity(const Entity &other):
    Entity( other.mName )
{
    //TODO: check which of these should be const
    mTickEnabled = false;
    mTickGroup = other.mTickGroup;
    mTickInterval = other.mTickInterval;
    mTickRegistered = false;
    mTickLast = 0.0f;

    mRegistered = false;
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
    if(nullptr == component){
        pt::log::err << "Entity::AddComponent(): Invalid null parameter\n";
    }

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
        Component* nullcomponent = nullptr;
        idx = pt::IndexOfInVector(mComponents, nullcomponent); //find an empty spot
        if(-1 < idx){
            mComponents[idx] = component;
        }else{
            mComponents.push_back(component);
        }

        //notify component
        entity::ComponentVisitor visitor(*this, *component);
        component->OnAddedToEntity(visitor);
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
    for(WorldComponent* wc : mWorldComponents){
        wc->Spawn();
    }
}

void Entity::
Despawn()
{
    for(WorldComponent* wc : mWorldComponents){
        wc->Despawn();
    }
}


void Entity::
AddWorldComponent(WorldComponent *component)
{
    assert(nullptr != component);
    if(nullptr == component){
        pt::log::err << "Entity::AddWorldComponent(): Invalid null parameter!\n";
        return;
    }

    if(pt::ContainedInVector(mWorldComponents, component)){
        pt::log::err << "Entity::AddWorldComponent(): Entity already contains component!\n";
        return;
    }

    mWorldComponents.push_back(component);
}


void Entity::
RemoveWorldComponent(WorldComponent *component)
{
    assert(nullptr != component);
    if(nullptr == component){
        pt::log::err << "Entity::RemoveWorldComponent(): Invalid null parameter!\n";
        return;
    }

    int idx = pt::IndexOfInVector(mWorldComponents, component);
    if(idx < 0){
        pt::log::err << "Entity::RemoveWorldComponent(): Entity does not contain Component!\n";
        return;
    }

    pt::RemoveElementInVector(mWorldComponents, idx);
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

