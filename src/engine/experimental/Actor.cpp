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
~Actor()
{}


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
        pt::log::err << subject->GetName() << ": failed to register Tick() function!\n";
        assert( false );
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
    auto lambda = [this]() -> void
    {
        pt::log::debug << "Actor::Spawn lambda executing\n";
        for( auto c : mComponents ){
            c->Spawn();
        }

        OnSpawned();
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
    pt::log::debug << "Actor::Spawn lambda added\n";
}


void Actor::
Despawn()
{
    auto lambda = [this]() -> void
    {
        OnDespawned();

        for( auto c : mComponents ){
            c->Despawn();
        }
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
}


void Actor::
AddComponent( Component *component )
{
    assert( nullptr != component );
    if( nullptr != component ){
        //-------------------------
        auto lambda = [this, component]() mutable -> void
        {
            bool suc = false;
            {
                MutexLockGuard lock ( mMutActorData );
                suc = pt::PushBackIfNotInVector( mComponents, component );
            }
            if( !suc ){
                pt::log::err << "Failed to add component '" << component->GetName()
                             << "'to actor '" << this->GetName() << "'\n";
            }
        };
        //-------------------------

        {
            MutexLockGuard lock ( mMutActorMessages );
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
        auto lambda = [this, component]() mutable -> void
        {
            int64_t idx = -1;
            {
                MutexLockGuard lock ( mMutActorData );
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
            MutexLockGuard lock ( mMutActorMessages );
            mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
        }
    }else{
        pt::log::warn << "Tried to remove 'nullptr' as component from actor '"
                      << this->GetName() << "'\n";
    }
}


//--------------------------------------------------

Actor* Actor::
GetParent()
{
    MutexLockGuard lock ( mMutActorData );
    return mParent;
}


const Actor *Actor::
GetParent() const
{
    MutexLockGuard lock ( mMutActorData );
    return mParent;
}


void Actor::
Tick( Actor& actor, float t, float dt)
{
    Actor::FlushMessages( actor );

    actor.TickComponents( t, dt );
    actor.OnTick( t, dt );
}


const math::float3 Actor::
GetPosition() const
{
    MutexLockGuard lock ( mMutActorData );
    return mRootComponent.GetPosition();
}


const math::float4 Actor::
GetOrientation() const
{
    MutexLockGuard lock ( mMutActorData );
    return mRootComponent.GetOrientation();
}


const math::float3 Actor::
GetScale() const
{
    MutexLockGuard lock ( mMutActorData );
    return mRootComponent.GetScale();
}


const math::float4x4 Actor::
GetTransform() const
{
    MutexLockGuard lock ( mMutActorData );
    return mRootComponent.GetTransform();
}


const math::float3 Actor::
GetWorldPosition() const
{
    MutexLockGuard lock ( mMutActorData );
    //TODO: get it from World
    return mRootComponent.GetWorldPosition();
}


const math::float4 Actor::
GetWorldOrientation() const
{
    MutexLockGuard lock ( mMutActorData );
    //TODO: get it from World
    assert( false );
    return math::float4::identity;
}


const math::float4x4 Actor::
GetWorldTransform() const
{
    MutexLockGuard lock ( mMutActorData );
    return mRootComponent.GetWorldTransform();
}


void Actor::
SetPosition( const pt::math::float3& pos )
{
    auto lambda = [this, pos]() -> void{
        MutexLockGuard lock ( mMutActorData );
        mRootComponent.SetPosition( pos );
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
}


void Actor::
SetOrientation( const pt::math::float4& orient )
{
    auto lambda = [this, orient]() -> void{
        MutexLockGuard lock ( mMutActorData );
        mRootComponent.SetOrientation( orient );
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
}


void Actor::
SetScale( const pt::math::float3& scale )
{
    auto lambda = [this, scale]() -> void{
        MutexLockGuard lock ( mMutActorData );
        mRootComponent.SetScale( scale );
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
}


void Actor::
SetRelativeTransform( const pt::math::float3& pos,
                      const pt::math::float4& orient,
                      const pt::math::float3& scale )
{
    auto lambda = [this, pos, orient, scale]() -> void{
        MutexLockGuard lock ( mMutActorData );
        mRootComponent.SetRelativeTransform( pos, orient, scale );
    };

    MutexLockGuard lock ( mMutActorMessages );
    mMessageQueue.GetInQueue()->addCallback( lambda, EventExecRule::TriggerOnce );
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

    auto messagesTrigger = q.GetProcQueueTrigger();
    auto messages = q.GetProcQueue();

    (*messagesTrigger)();
    messages->clear();
}


WorldComponent* Actor::
GetRootComponent()
{
    MutexLockGuard lock ( mMutActorData );
    return& mRootComponent;
}


void Actor::
TickComponents( float t, float dt)
{
    for( auto c : mComponents ){
        c->Tick( t, dt );
    }
}


std::vector<Component *> Actor::
GetComponents()
{
    std::vector< Component* > retval;
    retval = mComponents;
    return retval;
}


void Actor::
SetParent( Actor& parent )
{
    assert( false );
}


void Actor::
RemoveParent()
{
    assert( false );
}
