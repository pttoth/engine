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
{
    MutexLockGuard lock( mMutActorComponents );
    mRootComponent.EvOnTransformChanged.addCallback( this, &Actor::MarkRootComponentAsDirtyCallback );
}


Actor::
~Actor()
{}


const std::string &Actor::
GetName() const
{
    // No lock here, 'mName' is const
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
                psub->SetTickGroupState( group );
                psub->SetTickRegisteredState( true );
                pt::log::debug << psub->GetName() << ": Registered Tick() function!\n";
            }else{
                pt::log::err << psub->GetName() << ": Multiple Tick registrations for the same Actor!\n";
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        Services::GetScheduler2()->AddActor( subject, group );
        pt::log::debug << subject.GetName() << ": RegisterTick lambda added!\n";
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
                psub->SetTickGroupState( TickGroup::NO_GROUP );
                psub->SetTickRegisteredState( false );
                pt::log::debug << psub->GetName() << ": Unregistered Tick() function!\n";
            }else{
                pt::log::err << psub->GetName() << ": Multiple Tick unregistrations for the same Actor!\n";
                assert( false );
            }
        };
        subject.PostMessage( lambda );
        auto sched = Services::GetScheduler2();
        sched->RemoveDependenciesReferencingActor( subject ); //TODO: remove later as this has to be done by scheduler automatically
        sched->RemoveActor( subject );
        pt::log::debug << subject.GetName() << ": UnregisterTick lambda added!\n";
    }else{
        pt::log::err << subject.GetName() << ": Multiple Tick unregistrations for the same Actor!\n";
    }
}


void Actor::
AddTickDependency( Actor& subject, Actor& dependency )
{
    Services::GetScheduler2()->AddTickDependency( subject, dependency );
}


void Actor::
RemoveTickDependency( Actor& subject, Actor& dependency )
{
    Services::GetScheduler2()->RemoveTickDependency( subject, dependency );
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
    assert( !this->IsSpawned() );
    if( this->IsSpawned() ){
        pt::log::err << this->GetName() << ": Tried to spawn an already spawned actor!\n";
        return;
    }

    auto lambda = [this]() -> void
    {
        pt::log::debug << this->GetName() << ": Spawn lambda executing\n";
        {
            MutexLockGuard lock( mMutActorComponents );
            for( auto c : mComponents ){
                c->Spawn();
            }
        }
        OnSpawned();
        SetSpawnedState( true );
    };

    PostMessage( lambda );
    pt::log::debug <<  this->GetName() << ": Spawn lambda added\n";
}


void Actor::
Despawn()
{
    assert( this->IsSpawned() );
    if( !this->IsSpawned() ){
        pt::log::err << this->GetName() << ": Tried to despawn an actor that is not spawned!\n";
        return;
    }
    auto lambda = [this]() -> void
    {
        OnDespawned();
        {
            MutexLockGuard lock( mMutActorComponents );
            for( auto c : mComponents ){
                c->Despawn();
            }
        }
        SetSpawnedState( false );
    };

    PostMessage( lambda );
}


bool Actor::
IsSpawned()
{
    MutexLockGuard lock( mMutActorData );
    return mSpawned;
}


void Actor::
AddComponent( Component *component )
{
    assert( nullptr != component );
    if( nullptr == component ){
        pt::log::warn << "Tried to add 'nullptr' as component to actor '"
                      << this->GetName() << "'\n";
        return;
    }

    auto lambda = [this, component]() mutable -> void
    {
        bool suc = false;
        {
            MutexLockGuard lock( mMutActorComponents );
            suc = pt::PushBackIfNotInVector( mComponents, component );
        }
        if( !suc ){
            pt::log::err << "Failed to add component '" << component->GetName()
                         << "'to actor '" << this->GetName() << "'\n";
        }
    };
    PostMessage( lambda );
}


void Actor::
RemoveComponent( Component *component )
{
    assert( nullptr != component );
    if( nullptr == component ){
        pt::log::warn << "Tried to remove 'nullptr' as component from actor '"
                      << this->GetName() << "'\n";
        return;
    }

    auto lambda = [this, component]() -> void
    {
        int64_t idx = -1;
        {
            MutexLockGuard lock( mMutActorComponents );
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
    PostMessage( lambda );
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
    Actor::FlushMessages( actor );

    actor.OnPreTickComponents( t, dt );
    actor.TickComponents( t, dt );
    actor.OnPostTickComponents( t, dt );

    actor.OnTick( t, dt );
}


const math::float3 Actor::
GetPosition() const
{
    MutexLockGuard lock( mMutActorData );
    return mCachedPos;
}


const math::float4 Actor::
GetOrientation() const
{
    MutexLockGuard lock( mMutActorData );
    return mCachedOrient;
}


const math::float3 Actor::
GetScale() const
{
    MutexLockGuard lock( mMutActorData );
    return mCachedScale;
}


const math::float4x4 Actor::
GetTransform() const
{
    MutexLockGuard lock( mMutActorData );
    return mCachedTransform;
}


const math::float3 Actor::
GetWorldPosition() const
{
    //TODO: get it from World ( which mutex ??? )

    MutexLockGuard lock( mMutActorComponents );
    return mRootComponent.GetWorldPosition();
}


const math::float4 Actor::
GetWorldOrientation() const
{
    //TODO: get it from World ( which mutex ??? )
    assert( false );
    return math::float4::identity;
}


const math::float3 Actor::
GetWorldScale() const
{
    //TODO: get it from World ( which mutex ??? )
    assert( false );
    return math::float3( 1.0f, 1.0f, 1.0f );
}


const math::float4x4 Actor::
GetWorldTransform() const
{
    MutexLockGuard lock( mMutActorComponents );
    return mRootComponent.GetWorldTransform();
}


void Actor::
SetPosition( const pt::math::float3& pos )
{
    auto lambda = [this, pos]() -> void{
        {
            MutexLockGuard lock( mMutActorComponents );
            mRootComponent.SetPosition( pos );
        }
        CacheRootComponentData();
    };

    PostMessage( lambda );
}


void Actor::
SetOrientation( const pt::math::float4& orient )
{
    auto lambda = [this, orient]() -> void{
        {
            MutexLockGuard lock( mMutActorComponents );
            mRootComponent.SetOrientation( orient );
        }
        CacheRootComponentData();
    };

    PostMessage( lambda );
}


void Actor::
SetScale( const pt::math::float3& scale )
{
    auto lambda = [this, scale]() -> void{
        {
            MutexLockGuard lock( mMutActorComponents );
            mRootComponent.SetScale( scale );
        }
        CacheRootComponentData();
    };

    PostMessage( lambda );
}


void Actor::
SetRelativeTransform( const pt::math::float3& pos,
                      const pt::math::float4& orient,
                      const pt::math::float3& scale )
{
    auto lambda = [this, pos, orient, scale]() -> void{
        {
            MutexLockGuard lock( mMutActorComponents );
            mRootComponent.SetRelativeTransform( pos, orient, scale );
        }
        CacheRootComponentData();
    };

    PostMessage( lambda );
}


void Actor::
SetWorldPosition( const pt::math::float3 &pos )
{
    //TODO: finish
    assert( false );
}


void Actor::
SetWorldOrientation( const pt::math::float4 &orient )
{
    //TODO: finish
    assert( false );
}


void Actor::
SetWorldScale( const pt::math::float3 &scale )
{
    //TODO: finish
    assert( false );
}


void Actor::
SetWorldRelativeTransform( const pt::math::float3 &pos, const pt::math::float4 &orient, const pt::math::float3 &scale )
{
    //TODO: finish
    assert( false );
}


void Actor::
SetParent( Actor& parent )
{
    SetParentPtr( &parent );
}


void Actor::
RemoveParent()
{
    SetParentPtr( nullptr );
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
    // no mutex locking here, caller locks it
    return &mRootComponent;
}


const WorldComponent* Actor::
GetRootComponent() const
{
    // no mutex locking here, caller locks it
    return &mRootComponent;
}


void Actor::
TickComponents( float t, float dt )
{
    MutexLockGuard lock( mMutActorComponents );
    for( auto c : mComponents ){
        c->Tick( t, dt );
    }
}


void Actor::
OnPreTickComponents( float t, float dt )
{}


void Actor::
OnPostTickComponents( float t, float dt )
{
    if( RootComponentIsDirty() ){
        CacheRootComponentData();
    }
}


std::vector<Component*> Actor::
GetComponents()
{
    std::vector< Component* > retval;
    retval = mComponents;
    return retval;
}


std::vector<const Component*> Actor::
GetComponents() const
{
    std::vector< const Component* > retval;
    for( const Component* c : mComponents ){
        retval.push_back(c);
    }
    return retval;
}


void Actor::
SetParentPtr( Actor* parent )
{
    {
        MutexLockGuard lock( mMutActorData );
        mParent = parent;
    }
    UpdateWorldTransform();
}


void Actor::
UpdateWorldTransform()
{
    pt::log::warn << "Actor '" << this->GetName() << "'s UpdateWorldTransform() called, that is unimplemented!\n";
}


void Actor::
SetTickEnabledState( bool value )
{
    MutexLockGuard lock( mMutActorData );
    mTickEnabled = value;
}


void Actor::
SetTickRegisteredState( bool value )
{
    MutexLockGuard lock( mMutActorData );
    mTickRegistered = value;
}


void Actor::
SetTickGroupState( TickGroup value )
{
    MutexLockGuard lock( mMutActorData );
    mTickGroup = value;
}


void Actor::
SetSpawnedState( bool value )
{
    MutexLockGuard lock( mMutActorData );
    mSpawned = value;
}


void Actor::
CacheRootComponentData() const
{
    MutexLockGuard lock( mMutActorComponents );
    MutexLockGuard lock2( mMutActorData );
    mCachedPos          = mRootComponent.GetPosition();
    mCachedOrient       = mRootComponent.GetOrientation();
    mCachedScale        = mRootComponent.GetScale();
    mCachedTransform    = mRootComponent.GetTransform();
    mRootComponentIsDirty = false;
}


bool Actor::
RootComponentIsDirty() const
{
    MutexLockGuard lock( mMutActorData );
    return mRootComponentIsDirty;
}


void Actor::
MarkRootComponentAsDirtyCallback( WorldComponent* wc )
{
    MutexLockGuard lock( mMutActorData );
    mRootComponentIsDirty = true;
}
