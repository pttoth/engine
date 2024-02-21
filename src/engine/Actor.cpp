#include "engine/Actor.h"

#include "engine/Services.h"
#include "engine/Scheduler.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

#include <sstream>
#include <assert.h>

using namespace pt;
using namespace engine;


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
RegisterTickFunction( Actor& subject, TickGroup group )
{
    assert( !subject.IsTickRegistered() );
    if( !subject.IsTickRegistered() ){
        Actor* psub = &subject;
        auto lambda = [psub, group]() -> void
        {
            if( !psub->IsTickRegistered() && psub->GetTickGroup() == TickGroup::NO_GROUP ){
                psub->SetTickGroup_NoLock( group );
                psub->SetTickRegisteredState_NoLock( true );
                PT_LOG_DEBUG( psub->GetName() << ": Registered Tick() function!" );
            }else{
                pt::log::err << psub->GetName() << ": Multiple Tick registrations for the same Actor!\n";
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        Services::GetScheduler()->AddActor( subject, group );
        PT_LOG_DEBUG( subject.GetName() << ": RegisterTick lambda added!" );
    }else{
        pt::log::err << subject.GetName() << ": Multiple Tick registrations for the same Actor!\n";
    }
}


void Actor::
UnregisterTickFunction( Actor& subject )
{
    assert( subject.IsTickRegistered() );
    if( subject.IsTickRegistered() ){
        Actor* psub = &subject;
        auto lambda = [psub]() -> void
        {
            if( psub->IsTickRegistered() && psub->GetTickGroup() != TickGroup::NO_GROUP ){
                psub->SetTickGroup_NoLock( TickGroup::NO_GROUP );
                psub->SetTickRegisteredState_NoLock( false );
                PT_LOG_DEBUG( psub->GetName() << ": Unregistered Tick() function!" );
            }else{
                pt::log::err << psub->GetName() << ": Multiple Tick unregistrations for the same Actor!\n";
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        auto sched = Services::GetScheduler();
        sched->RemoveDependenciesReferencingActor( subject ); //TODO: remove later as this has to be done by scheduler automatically
        sched->RemoveActor( subject );
        PT_LOG_DEBUG( subject.GetName() << ": UnregisterTick lambda added!" );
    }else{
        pt::log::err << subject.GetName() << ": Multiple Tick unregistrations for the same Actor!\n";
    }
}


void Actor::
AddTickDependency( Actor& subject, Actor& dependency )
{
    Services::GetScheduler()->AddTickDependency( subject, dependency );
}


void Actor::
RemoveTickDependency( Actor& subject, Actor& dependency )
{
    Services::GetScheduler()->RemoveTickDependency( subject, dependency );
}


float Actor::
GetTickInterval() const
{
    MutexLockGuard lock( mMutActorData );
    return mTickInterval;
}


TickGroup Actor::
GetTickGroup() const
{
    MutexLockGuard lock( mMutActorData );
    return mTickGroup;
}


bool Actor::
IsTickEnabled() const
{
    MutexLockGuard lock( mMutActorData );
    return mTickEnabled;
}


bool Actor::
IsTickRegistered() const
{
    MutexLockGuard lock( mMutActorData );
    return mTickRegistered;
}


void Actor::
Spawn()
{
    auto lambda = [this]() -> void
    {
        PT_LOG_DEBUG( this->GetName() << ": Spawn lambda executing" );

        if( this->IsSpawned() ){
            pt::log::err << this->GetName() << ": Tried to spawn an already spawned actor!\n";
            return;
        }

        {
            MutexLockGuard lock( mMutActorData );
            for( auto c : mComponents ){
                c->Spawn();
            }
        }
        OnSpawned();
        SetSpawnedState_NoLock( true );
    };

    PostMessage( lambda );
    PT_LOG_DEBUG( this->GetName() << ": Spawn lambda added" );
}


void Actor::
Despawn()
{
    auto lambda = [this]() -> void
    {
        if( !this->IsSpawned() ){
            pt::log::err << this->GetName() << ": Tried to despawn an actor that is not spawned!\n";
            return;
        }

        OnDespawned();
        {
            MutexLockGuard lock( mMutActorData );
            for( auto c : mComponents ){
                c->Despawn();
            }
        }
        SetSpawnedState_NoLock( false );
    };

    PostMessage( lambda );
}


bool Actor::
IsSpawned()
{
    MutexLockGuard lock( mMutActorData );
    return mSpawned;
}


Actor* Actor::
GetParent()
{
    MutexLockGuard lock( mMutActorData );
    return mParent;
}


const Actor *Actor::
GetParent() const
{
    MutexLockGuard lock( mMutActorData );
    return mParent;
}


void Actor::
Tick( Actor& actor, float t, float dt )
{
    Actor::FlushMessages_NoDelay( actor );

    actor.OnPreTickComponents( t, dt );
    actor.TickComponents_NoDelay( t, dt );
    actor.OnPostTickComponents( t, dt );

    actor.OnTick( t, dt );
    //TODO: check if position is dirty here (there was no position update during OnTick() )
    //  if so, update the position according to parent (it may have moved since)

    //TODO: check if this Actor has changed its Transform during Tick
    //  if so, fire an event, that it changed position
}


const math::float3 Actor::
GetPosition() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetPosition();
}


const math::float4 Actor::
GetOrientation() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetOrientation();
}


const math::float3 Actor::
GetScale() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetScale();
}


const math::float4x4 Actor::
GetTransform() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetTransform();
}


const math::float3 Actor::
GetWorldPosition() const
{
    //TODO: get it from World ( which mutex ??? )

    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetWorldPosition();
}


const math::float4 Actor::
GetWorldOrientation() const
{
    //TODO: get it from World ( which mutex ??? )
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float3 Actor::
GetWorldScale() const
{
    //TODO: get it from World ( which mutex ??? )
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float4x4 Actor::
GetWorldTransform() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetWorldTransform();
}


void Actor::
SetPosition( const math::float3& pos )
{
    auto lambda = [this, pos]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetPosition( pos );
        }
    };

    PostMessage( lambda );
}


void Actor::
SetOrientation( const math::float4& orient )
{
    auto lambda = [this, orient]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetOrientation( orient );
        }
    };

    PostMessage( lambda );
}


void Actor::
SetScale( const math::float3& scale )
{
    auto lambda = [this, scale]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetScale( scale );
        }
    };

    PostMessage( lambda );
}


void Actor::
SetRelativeTransform( const math::float3& pos,
                      const math::float4& orient,
                      const math::float3& scale )
{
    auto lambda = [this, pos, orient, scale]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetRelativeTransform( pos, orient, scale );
        }
    };

    PostMessage( lambda );
}


void Actor::
SetWorldPosition( const math::float3 &pos )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetWorldOrientation( const math::float4 &orient )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetWorldScale( const math::float3 &scale )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetWorldRelativeTransform( const math::float3 &pos, const math::float4 &orient, const math::float3 &scale )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetParent( Actor& parent )
{
    auto lambda = [this, &parent]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            SetParentPtr_NoLock( &parent );
        }
    };

    PostMessage( lambda );
}


void Actor::
RemoveParent()
{
    auto lambda = [this]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            SetParentPtr_NoLock( nullptr );
        }
    };

    PostMessage( lambda );
}


void Actor::
FlushMessages_NoDelay( Actor& actor )
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


void Actor::
AddComponent_NoLock( Component* component )
{
    if( nullptr == component ){
        pt::log::warn << "Tried to add 'nullptr' as component to actor '"
                      << this->GetName() << "'\n";
        assert( nullptr != component );
        return;
    }

    bool suc = false;
    suc = pt::PushBackIfNotInVector( mComponents, component );
    if( !suc ){
        pt::log::err << "Failed to add component '" << component->GetName()
                     << "'to actor '" << this->GetName() << "'\n";
        assert( suc );
    }
}


void Actor::
RemoveComponent_NoLock( Component* component )
{
    if( nullptr == component ){
        pt::log::warn << "Tried to remove 'nullptr' as component from actor '"
                      << this->GetName() << "'\n";
        assert( nullptr != component );
        return;
    }

    int64_t idx = pt::IndexOfInVector( mComponents, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mComponents, idx );
    }else {
        pt::log::err << "Tried to remove a non-attached component '" << component->GetName()
                     << "' from actor '" << this->GetName() << "'\n";
        assert( -1 < idx );
    }
}


void Actor::
AddDrawableComponent_NoLock( RealComponent* component )
{
    if( nullptr == component ){
        pt::log::warn << "Tried to add 'nullptr' as drawable component to actor '"
                      << this->GetName() << "'\n";
        assert( nullptr != component );
        return;
    }

    int64_t idx = pt::IndexOfInVector( mRealComponents, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mRealComponents, idx );
    }else {
        pt::log::err << "Tried to remove a non-attached drawable component '" << component->GetName()
                     << "' from actor '" << this->GetName() << "'\n";
        assert( -1 < idx );
    }
}


void Actor::
RemoveDrawableComponent_NoLock( RealComponent* component )
{
    if( nullptr == component ){
        pt::log::warn << "Tried to remove 'nullptr' as drawable component from actor '"
                      << this->GetName() << "'\n";
        assert( nullptr != component );
        return;
    }

    int64_t idx = pt::IndexOfInVector( mRealComponents, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mRealComponents, idx );
    }else {
        pt::log::err << "Tried to remove a non-attached drawable component '" << component->GetName()
                     << "' from actor '" << this->GetName() << "'\n";
        assert( -1 < idx );
    }
}


WorldComponent* Actor::
GetRootComponent_NoLock()
{
    return &mRootComponent;
}


const WorldComponent* Actor::
GetRootComponent_NoLock() const
{
    return &mRootComponent;
}


void Actor::
TickComponents_NoDelay( float t, float dt )
{
    MutexLockGuard lock( mMutActorData );
    for( auto c : mComponents ){
        c->Tick( t, dt );
    }
}


void Actor::
OnPreTickComponents( float t, float dt )
{}


void Actor::
OnPostTickComponents( float t, float dt )
{}


std::vector<Component*> Actor::
GetComponents_NoLock()
{
    std::vector< Component* > retval;
    retval = mComponents;
    return retval;
}


std::vector<const Component*> Actor::
GetComponents_NoLock() const
{
    std::vector< const Component* > retval;
    for( const Component* c : mComponents ){
        retval.push_back(c);
    }
    return retval;
}


void Actor::
SetParentPtr_NoLock( Actor* parent )
{
    //TODO: possibly missing handling of children here
    mParent = parent;
    UpdateWorldTransform_NoLock();
}


void Actor::
UpdateWorldTransform_NoLock()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetTickEnabled_NoLock( bool value )
{
    mTickEnabled = value;
}


void Actor::
SetTickRegisteredState_NoLock( bool value )
{
    mTickRegistered = value;
}


void Actor::
SetTickGroup_NoLock( TickGroup value )
{
    mTickGroup = value;
}


void Actor::
SetSpawnedState_NoLock( bool value )
{
    mSpawned = value;
}

