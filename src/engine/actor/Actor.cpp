#include "engine/actor/Actor.h"

#include "engine/Services.h"
#include "engine/service/Scheduler.h"
#include "pt/logging.h"
#include "pt/utility.hpp"
#include <sstream>
#include <assert.h>

using namespace pt;
using namespace engine;


const std::string Actor::mRootComponentName{ "rootComponent" };




std::string Actor::
GenerateComponentName( const Actor& actor, const std::string& component_name )
{
    std::stringstream ss;
    ss << actor.GetName() << "." << component_name;
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
    if( !mSwapped ){        // (0)
        return &mTrigger1;
    }else{                  // (1)
        return &mTrigger2;
    }
}


pt::EventTrigger<> *Actor::DoubleBufferedEventQueue::
GetProcQueueTrigger()
{
    if( !mSwapped ){        // (0)
        return &mTrigger2;
    }else{                  // (1)
        return &mTrigger1;
    }
}


void Actor::DoubleBufferedEventQueue::
SwapBuffers()
{
    mSwapped = ( mSwapped + 1 ) %2;
}


pt::Event<>* Actor::DoubleBufferedEventQueue::
GetInQueue()
{
    if( !mSwapped ){        // (0)
        return &mMessageQueue1;
    }else{                  // (1)
        return &mMessageQueue2;
    }
}


pt::Event<>* Actor::DoubleBufferedEventQueue::
GetProcQueue()
{
    if( !mSwapped ){        // (0)
        return &mMessageQueue2;
    }else{                  // (1)
        return &mMessageQueue1;
    }
}


//------------------------------------------------------------------------------------------------------------------------

Actor::
Actor( const std::string& name ):
    mName( name ),
    mRootComponent( NewPtr<PositionComponent>( Actor::GenerateComponentName( *this, mRootComponentName ) ) )
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
RegisterTickFunction( ActorPtr subject, TickGroup group )
{
    if( subject ){
        Actor::RegisterTickFunction( *subject.get(), group );
    }else{
        PT_LOG_ERR( "Tried to register non-existent Actor's Tick function!"  );
        assert( nullptr != subject );
    }
}


void Actor::
RegisterTickFunction( Actor& subject, TickGroup group )
{
    assert( !subject.IsTickRegistered() );
    if( !subject.IsTickRegistered() ){
        Actor* psub = &subject;
        auto lambda = [psub, group]() -> void
        {
            if( group == TickGroup::NO_GROUP ){
                PT_LOG_ERR( "Invalid TickGroup, while trying to tick-register Actor'" << psub->GetName() << "'" );
                return;
            }

            if( !psub->IsTickRegistered() && psub->GetTickGroup() == TickGroup::NO_GROUP ){
                psub->SetTickGroup_NoLock( group );
                psub->SetTickRegisteredState_NoLock( true );
                PT_LOG_DEBUG( psub->GetName() << ": Registered Tick() function!" );
            }else{
                PT_LOG_ERR( psub->GetName() << ": Multiple Tick registrations for the same Actor!"  );
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        Services::GetScheduler()->AddActor( subject, group );
        PT_LOG_DEBUG( subject.GetName() << ": RegisterTick lambda added!" );
    }else{
        PT_LOG_ERR( subject.GetName() << ": Multiple Tick registrations for the same Actor!" );
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
                PT_LOG_ERR( psub->GetName() << ": Multiple Tick unregistrations for the same Actor!" );
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        auto sched = Services::GetScheduler();
        sched->RemoveDependenciesReferencingActor( subject ); //TODO: remove later as this has to be done by scheduler automatically
        sched->RemoveActor( subject );
        PT_LOG_DEBUG( subject.GetName() << ": UnregisterTick lambda added!" );
    }else{
        PT_LOG_ERR( subject.GetName() << ": Multiple Tick unregistrations for the same Actor!" );
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
            PT_LOG_ERR( this->GetName() << ": Tried to spawn an already spawned actor!" );
            return;
        }

        {
            MutexLockGuard lock( mMutActorData );
            for( auto c : mComponents ){
                c->Spawn();
            }
        }
        this->OnSpawned();
        this->SetSpawnedState_NoLock( true );
    };

    this->PostMessage( lambda );
    PT_LOG_DEBUG( this->GetName() << ": Spawn lambda added" );
}


void Actor::
Despawn()
{
    auto lambda = [this]() -> void
    {
        PT_LOG_DEBUG( this->GetName() << ": Despawn lambda executing" );
        if( !this->IsSpawned() ){
            PT_LOG_ERR( this->GetName() << ": Tried to despawn an actor that is not spawned!" );
            return;
        }

        this->OnDespawned();
        {
            MutexLockGuard lock( mMutActorData );
            for( auto c : mComponents ){
                c->Despawn();
            }
        }
        this->SetSpawnedState_NoLock( false );
    };

    this->PostMessage( lambda );
    PT_LOG_DEBUG( this->GetName() << ": Despawn lambda added" );
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
    return GetParent_NoLock();
}


const Actor *Actor::
GetParent() const
{
    MutexLockGuard lock( mMutActorData );
    return GetParent_NoLock();
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


const math::float4x4 Actor::
GetRotationMtx() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetRotationMtx();
}


/*
const math::float4 Actor::
GetOrientation() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetOrientation();
}
*/

const math::float3 Actor::
GetScale() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetScale();
}


const math::float4x4 Actor::
GetRelativeTransform() const
{
    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetRelativeTransform();
}


const math::float3 Actor::
GetWorldPosition() const
{
    //TODO: get it from World ( which mutex ??? )

    MutexLockGuard lock( mMutActorData );
    return GetRootComponent_NoLock()->GetWorldPosition();
}

/*
const math::float4 Actor::
GetWorldOrientation() const
{
    //TODO: get it from World ( which mutex ??? )
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float4x4 Actor::
GetWorldRotationMtx() const
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
*/

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

/*
void Actor::
SetOrientation( const math::float4& orient );
{
    auto lambda = [this, orient]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetOrientation( orient );
        }
    };

    PostMessage( lambda );
}
*/


void Actor::
SetRotation( const math::FRotator& rotator )
{
    auto lambda = [this, rotator]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetRotation( rotator );
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
                      const math::FRotator& orient,
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
SetRelativeTransform( const math::float4x4& transform )
{
    auto lambda = [this, transform]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetRelativeTransform( transform );
        }
    };

    PostMessage( lambda );
}


void Actor::
SetWorldPosition( const math::float3& pos )
{
    auto lambda = [this, pos]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetWorldPosition( pos );
        }
    };

    PostMessage( lambda );
}

/*
void Actor::
SetWorldOrientation( const math::FRotator &orient )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}


void Actor::
SetWorldRotation(const math::FRotator &rotator)
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
SetWorldTransform( const math::float3 &pos, const math::float4 &orient, const math::float3 &scale )
{
    //TODO: finish
    PT_UNIMPLEMENTED_FUNCTION
}
*/

void Actor::
SetWorldTransform( const math::float4x4& transform )
{
    auto lambda = [this, transform]() -> void{
        {
            MutexLockGuard lock( mMutActorData );
            GetRootComponent_NoLock()->SetWorldTransform( transform );
        }
    };

    PostMessage( lambda );
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


bool Actor::
CreateRenderContext()
{
    // don't use the message queue here
    // logically this can only be called when the Actor is disconnected
    if( mContextExists || mSpawned ){ //TODO: verify, if there's a case, where the Actor can be in Spawned state without having its rendercontext initialized!
        std::stringstream ss;
        ss << "Called CreateRenderContext for Actor '" << this->GetName()
           << "'at an invalid time (HasContext: " << mContextExists << ", Spawned: " << mSpawned << ")";
        throw std::logic_error( ss.str() ); // @TODO: don't throw exceptions, fail gracefully instead
    }

    // @TODO: create render context for components here

    mContextExists = OnCreateRenderContext();

#ifndef NDEBUG
    if( !mContextExists ){
        std::stringstream ss;
        ss << "Failed to create rendercontext for Actor '" << this->GetName();
        throw std::runtime_error( ss.str() );
    }
#endif
    return mContextExists;
}


void Actor::
DestroyRenderContext()
{
    // don't use the message queue here
    // logically this can only be called when the Actor is disconnected
    if( !mContextExists || mSpawned ){
        std::stringstream ss;
        ss << "Called DestroyRenderContext for Actor '" << this->GetName()
           << "'at an invalid time (HasContext: " << mContextExists << ", Spawned: " << mSpawned << ")";
        throw std::logic_error( ss.str() );
    }
    OnDestroyRenderContext();
    mContextExists = false;
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
AddComponent_NoLock( ComponentPtr component )
{
    if( nullptr == component ){
        PT_LOG_WARN( "Tried to add 'nullptr' as component to actor '" << this->GetName() << "'" );
        assert( nullptr != component );
        return;
    }

    bool suc = false;
    suc = pt::PushBackIfNotInVector( mComponents, component );
    if( !suc ){
        PT_LOG_ERR( "Failed to add component '" << component->GetName() << "'to actor '" << this->GetName() << "'" );
        assert( suc );
    }
}


void Actor::
RemoveComponent_NoLock( ComponentPtr component )
{
    if( nullptr == component ){
        PT_LOG_WARN( "Tried to remove 'nullptr' as component from actor '" << this->GetName() << "'" );
        assert( nullptr != component );
        return;
    }

    int64_t idx = pt::IndexOfInVector( mComponents, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mComponents, idx );
    }else {
        PT_LOG_ERR( "Tried to remove a non-attached component '" << component->GetName() << "' from actor '" << this->GetName() << "'" );
        assert( -1 < idx );
    }
}


Actor* Actor::
GetParent_NoLock()
{
    return mParent;
}


const Actor* Actor::
GetParent_NoLock() const
{
    return mParent;
}


WorldComponentPtr Actor::
GetRootComponent_NoLock()
{
    return mRootComponent;
}


ConstWorldComponentPtr Actor::
GetRootComponent_NoLock() const
{
    return mRootComponent;
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


std::vector<ComponentPtr> Actor::
GetComponents_NoLock()
{
    std::vector< ComponentPtr > retval;
    retval = mComponents;
    return retval;
}


std::vector<ConstComponentPtr> Actor::
GetComponents_NoLock() const
{
    std::vector< ConstComponentPtr > retval;
    for( ConstComponentPtr c : mComponents ){
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
    // @TODO: implement cached world transform
    //PT_UNIMPLEMENTED_FUNCTION
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

