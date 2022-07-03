#include "engine/Component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "engine/WorldComponent.h"
#include "engine/RealComponent.h"

#include "engine/Services.h"
#include "engine/EngineControl.h"
#include "engine/Scheduler.h"

#include <assert.h>

#include <sstream>

using namespace engine;

std::string
GenerateComponentName(const std::string& sourcename)
{
    std::stringstream ss;
    //TODO: cross-reference name with existing name pool
    //      find a new, unused name for the entity
    //      (remove numbers from the end and append a new number instead)

    //TODO: remove this temp solution
    if( 0 == sourcename.length() ){
        ss << "AutoGenName"; //TODO: use a cross-referenced free number instead
    }else{
        ss << sourcename << "_copy";
    }
    return ss.str();
}


Component::
Component(const std::string& name):
    mName(name)
{}


Component::
Component(const Component& other):
    Component( GenerateComponentName( other.GetName() ) )
{}


Component::
~Component()
{}


void Component::
OnAddedToEntity(entity::ComponentVisitor&)
{}


void Component::
OnRemovedFromEntity(entity::ComponentVisitor&)
{}


void Component::
Spawn()
{
    OnSpawned();
}


void Component::
Despawn()
{
    OnDespawned();
}


const std::string& Component::
GetName() const
{
    return mName;
}


void Component::
enableTick()
{
    mTickEnabled = true;
}


void Component::
disableTick()
{
    mTickEnabled = false;
}

void Component::RegisterTickFunction()
{
    mTickRegistered = true;
    Services::GetScheduler()->AddTicker(*this);
}


float Component::
GetTickInterval() const
{
    return mTickInterval;
}


Ticker::Group Component::
GetTickGroup() const
{
    return mTickGroup;
}


bool Component::IsTickEnabled() const
{
    return mTickEnabled;
}


bool Component::IsTickRegistered() const
{
    return mTickRegistered;
}
