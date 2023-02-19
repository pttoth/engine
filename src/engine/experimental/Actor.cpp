#include "engine/experimental/Actor.h"

#include "engine/experimental/ComponentVisitor.h"

#include "engine/Services.h"
#include "engine/experimental/Scheduler.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

#include <sstream>
#include <assert.h>

using namespace pt;
using namespace engine;
using namespace experimental;


const std::string Actor::mRootComponentName{ "rootComponent" };




std::string Actor::
GenerateComponentName( const Actor& actor, const std::string& component_name_ )
{
    std::string retval;
    retval.reserve( actor.GetName().length() + 1 + component_name_.length() + 1 ); // '<actor>.<component>' + \0

    std::stringstream ss;
    ss << retval << "." << component_name_;
    return ss.str();
}


void Actor::
AddedWorldComponent(WorldComponent *component)
{
    assert( false );
}


void Actor::
RemovedWorldComponent(WorldComponent *component)
{
    assert( false );
}


//------------------------------------------------------------------------------------------------------------------------

Actor::DoubleBufferedEventQueue::
DoubleBufferedEventQueue():
    mMessageQueue1( mTrigger1 ),
    mMessageQueue2( mTrigger2 )
{}


Actor::DoubleBufferedEventQueue::
~DoubleBufferedEventQueue()
{}


pt::EventTrigger<> *Actor::DoubleBufferedEventQueue::
GetInQueueTrigger()
{
    std::array< pt::EventTrigger<>*, 2 > triggers( { &mTrigger1, &mTrigger2 } );
    uint8_t idx = ( mSwapped ) %2;
    return triggers.at( idx );
}


pt::EventTrigger<> *Actor::DoubleBufferedEventQueue::
GetProcQueueTrigger()
{
    std::array< pt::EventTrigger<>*, 2 > triggers( { &mTrigger1, &mTrigger2 } );
    uint8_t idx = ( mSwapped+1 ) %2;
    return triggers.at( idx );
}


void Actor::DoubleBufferedEventQueue::
SwapBuffers()
{
    mSwapped = ( mSwapped + 1 ) %2;
}


pt::Event<>* Actor::DoubleBufferedEventQueue::
GetInQueue()
{
    std::array< pt::Event<>*, 2 > queues( { &mMessageQueue1, &mMessageQueue2 } );
    uint8_t idx = ( mSwapped ) %2;
    return queues.at( idx );
}


pt::Event<>* Actor::DoubleBufferedEventQueue::
GetProcQueue()
{
    std::array< pt::Event<>*, 2 > queues( { &mMessageQueue1, &mMessageQueue2 } );
    uint8_t idx = ( mSwapped +1 ) %2;
    return queues.at( idx );
}


//------------------------------------------------------------------------------------------------------------------------

Actor::
Actor( const std::string& name ):
    mName( name ),
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
{}


Actor::
Actor( const Actor& other ):
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
{}


Actor::
Actor( Actor&& source ):
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
{}


Actor::
~Actor()
{}


Actor &Actor::
operator=( const Actor& other )
{
    assert( false );
}


Actor &Actor::
operator=( Actor&& source )
{
    assert( false );
}


bool Actor::
operator==( const Actor& other ) const
{
    assert( false );
}


const std::string &Actor::
GetName() const
{
    return mName;
}


void Actor::
RegisterTickFunction( Actor* subject, TickGroup group )
{
    assert( nullptr != subject );
    if( !subject->IsTickRegistered() ){
        subject->mTickGroup = group;
        Services::GetScheduler2()->AddActor( *subject );
        subject->mTickRegistered = true;
    }else{
        assert(false); //TODO: throw instead
    }
}


void Actor::
UnregisterTickFunction( Actor* subject )
{
    assert( nullptr != subject );

}


void Actor::
AddTickDependency( Actor* subject, Actor* dependency )
{
    assert( nullptr != subject );

}


void Actor::
RemoveTickDependency( Actor* subject, Actor* dependency )
{
    assert( nullptr != subject );

}


float Actor::
GetTickInterval() const
{
    return mTickInterval;
}


TickGroup Actor::
GetTickGroup() const
{
    return mTickGroup;
}


bool Actor::
IsTickEnabled() const
{
    return mTickEnabled;
}


bool Actor::
IsTickRegistered() const
{
    return mTickRegistered;
}


void Actor::
Spawn()
{
    for( auto c : mComponents ){
        c->Spawn();
    }

    OnSpawned();
}


void Actor::
Despawn()
{
    OnDespawned();

    for( auto c : mComponents ){
        c->Despawn();
    }
}


void Actor::
AddComponent( Component *component )
{
    assert( nullptr != component );
    if( nullptr != component ){
        //-------------------------
        auto lambda = [&]() // TODO: check lambda type
        {
            bool suc = false;
            {
                MutexLock lock ( mMutActorData );
                suc = pt::PushBackIfNotInVector( mComponents, component );
            }
            if( !suc ){
                pt::log::err << "Failed to add component '" << component->GetName()
                             << "'to actor '" << this->GetName() << "'\n";
            }
        };
        //-------------------------

        {
            MutexLock lock ( mMutActorMessages );
            mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
        }
    }else{
        pt::log::warn << "Tried to add 'nullptr' as component to actor '"
                      << this->GetName() << "'\n";
    }
}


void Actor::
RemoveComponent( Component *component )
{
    assert( nullptr != component );
    if( nullptr != component ){
        //-------------------------
        auto lambda = [&]() // TODO: check lambda type
        {
            int64_t idx = -1;
            {
                MutexLock lock ( mMutActorData );
                idx = pt::IndexOfInVector( mComponents, component );
                if( -1 < idx ){
                    pt::RemoveElementInVector( mComponents, idx );
                }
            }
            if( idx < 0 ){
                pt::log::err << "Tried to remove a non-attached component '" << component->GetName()
                             << "' from actor '" << this->GetName() << "'\n";
            }
        };
        //-------------------------

        {
            MutexLock lock ( mMutActorMessages );
            mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
        }
    }else{
        pt::log::warn << "Tried to remove 'nullptr' as component from actor '"
                      << this->GetName() << "'\n";
    }
}


WorldComponent* Actor::
GetRootComponent()
{
    MutexLock lock ( mMutActorData );
    return &mRootComponent;
}


/*
std::vector<Component *> Actor::
GetComponents()
{
    MutexLock lock ( mMutActorData );
    std::vector< Component* > retval;
    retval = mComponents;
    return retval;
}
*/


//--------------------------------------------------

Actor* Actor::
GetParent()
{
    MutexLock lock ( mMutActorData );
    return mParent;
}


void Actor::
Tick(float t, float dt)
{
    assert( false );
}


void Actor::
FlushMessages( Actor& actor )
{
    MutexLockGuard( actor.mMutActorMessageProcessing );
    DoubleBufferedEventQueue& q = actor.mMessageQueue;
    {
        MutexLockGuard( actor.mMutActorMessages );
        q.SwapBuffers();
    }
    auto messages = q.GetProcQueueTrigger();
    (*messages)();
}


void Actor::
TickComponents()
{
    assert( false );
}


void Actor::
SetParent()
{
    assert( false );
}


void Actor::
RemoveParent()
{
    assert( false );
}
