/** -----------------------------------------------------------------------------
  * FILE:    Actor.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Core class responsible for supporting concurrency.
  * DESCRIPTION: Each Actor is to be expected that they MAY run concurrently to
  *   each other, provided there are no TickDependencies are set up for them.
  *   Defines a message queue for every instance. Publicly available functions
  *   don't operate on the Actors' data directly, rather every one of them pushes
  *   a lambda into the message queue, which will be executed on the next Tick()
  *   in a FIFO manner. Functions querying data from the Actor DO access data directly.
  *   This data may be fresh, or out of date by one frame, depending on whether the Actor
  *   has Ticked in the frame by the time the call was made. (Use TickDependencies
  *   to control the Ticking order of Actors.)
  *   TODO: fix issue with queued lambdas and serialization
  *
  *   Actors are never to be copied.
  *   Move semantics aren't supported. Use shared pointers, custom-provided allocators, etc.
  *   'Operator==()' is deleted. Actor names are unique, equality checks happen by comparing name.
  *
  *   TODO: add desc of Tick windows (PREPHYSICS, DURINGPHYSICS, POSTPHYSICS)
  *   TODO: add desc of Tickdependencies
  *   TODO: add desc of Components' Tick timing
  *   Function tag naming convention:
  *     <no tag>: Uses mutexes and message queue (except query functions).
  *     _NoDelay: Uses mutexes, but does not use the message queue.
  *                Public data query functions aren't marked, but are this category.
  *      @TODO: fix rules
  *                Create/DestroyRenderContext() doesn't fit the rule
  *
  *     _NoLock:  Does not use mutexes, neither message queues.
  *                These functions directly access data immediately.
  *
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Def.h"

#include "engine/component/RealComponent.h"
#include "engine/component/PositionComponent.h"
#include "pt/alias.h"
#include "pt/event.hpp"
#include "pt/macros.h"
#include <vector>

// @TODO: add LookAt() function from Camera

namespace engine{

enum class TickGroup{
    NO_GROUP = 0, //TODO: probably needs deletion
    PREPHYSICS,
    DURINGPHYSICS,
    POSTPHYSICS,
};

PT_FORWARD_DECLARE_CLASS( Actor )

class Actor
{
    // --------------------------------------------------
    //  DoubleBufferedEventQueue
    // --------------------------------------------------
    class DoubleBufferedEventQueue
    {
        pt::EventTrigger<> mTrigger1;
        pt::EventTrigger<> mTrigger2;
        pt::Event<> mMessageQueue1;
        pt::Event<> mMessageQueue2;

        uint8_t mSwapped = 0;     //can only be 0 or 1
    public:
        DoubleBufferedEventQueue();
        DoubleBufferedEventQueue( const DoubleBufferedEventQueue& other ) = delete;
        DoubleBufferedEventQueue( DoubleBufferedEventQueue&& source ) = delete;
        virtual ~DoubleBufferedEventQueue();
        DoubleBufferedEventQueue& operator=( const DoubleBufferedEventQueue& other ) = delete;
        DoubleBufferedEventQueue& operator=( DoubleBufferedEventQueue&& source ) = delete;
        bool operator==( const DoubleBufferedEventQueue& other ) const = delete;

        pt::EventTrigger<>* GetInQueueTrigger();
        pt::EventTrigger<>* GetProcQueueTrigger();
        pt::Event<>* GetInQueue();
        pt::Event<>* GetProcQueue();

        void SwapBuffers();
    };
    // --------------------------------------------------
public:
    Actor( const std::string& name );
    Actor( const Actor& other ) = delete;
    Actor( Actor&& source ) = delete;

    virtual ~Actor();

    Actor& operator=( const Actor& other ) = delete;
    Actor& operator=( Actor&& source ) = delete;

    bool operator==( const Actor& other ) const = delete;

    //-----

    const std::string& GetName() const;

    static void RegisterTickFunction( ActorPtr subject, TickGroup group = TickGroup::DURINGPHYSICS );
    static void RegisterTickFunction( Actor& subject, TickGroup group = TickGroup::DURINGPHYSICS );
    static void UnregisterTickFunction( Actor& subject );
    static void AddTickDependency( Actor& subject, Actor& dependency );
    static void RemoveTickDependency( Actor& subject, Actor& dependency );

    /**
     * @brief SetTickInterval
     * @param interval: unit (ms)
     * @throws 'std::out_of_range' on negative values
     */
    static void SetTickInterval( Actor& subject, float interval );

    template<class TMessageType>
    void PostMessage( TMessageType&& message ){
        pt::MutexLockGuard lock ( mMutActorMessages );
        mMessageQueue.GetInQueue()->addCallback( message, pt::EventExecRule::TriggerOnce );
    }

    virtual float GetTickInterval() const;
    virtual TickGroup GetTickGroup() const;
    virtual bool IsTickEnabled() const;
    virtual bool IsTickRegistered() const;

    void Spawn();
    void Despawn();
    bool IsSpawned();

    Actor* GetParent();
    const Actor* GetParent() const;

    static void Tick( Actor& actor, float t, float dt );

    const math::float3    GetPosition() const;
    //const math::float4    GetOrientation() const;     //TODO: implement after having Quaternion class
    const math::float4x4  GetRotationMtx() const;
    const math::float3    GetScale() const;
    const math::float4x4  GetRelativeTransform() const;

    const math::float3    GetWorldPosition() const;
    //const math::float4    GetWorldOrientation() const;    // TODO: implement after having Quaternion class
    //const math::float4x4  GetWorldRotationMtx() const;    // TODO: remove, after implemented Quaternion
    //const math::float3    GetWorldScale() const;          // TODO: decide whether to remove...
    const math::float4x4  GetWorldTransform() const;

    void SetPosition( const math::float3& pos );
    //void SetOrientation( const math::float4& orient );    //TODO: implement after having Quaternion class
    void SetRotation( const math::FRotator& rotator );
    void SetScale( const math::float3& scale );
    void SetRelativeTransform( const math::float3& pos, const math::FRotator& orient, const math::float3& scale );
    void SetRelativeTransform( const math::float4x4& transform );

    void SetWorldPosition( const math::float3& pos );
    //void SetWorldOrientation( const math::float4& orient );    //TODO: implement after having Quaternion class
    //void SetWorldRotation( const math::FRotator& rotator );
    //void SetWorldScale( const math::float3& scale );
    //void SetWorldTransform( const math::float3& pos, const math::float4& orient, const math::float3& scale );
    void SetWorldTransform( const math::float4x4& transform );

    void SetParent( Actor& parent );
    void RemoveParent();

    bool CreateRenderContext();
    void DestroyRenderContext();

protected:
    static void FlushMessages_NoDelay( Actor& actor );

    void AddComponent_NoLock( ComponentPtr component );
    void RemoveComponent_NoLock( ComponentPtr component );

          Actor*    GetParent_NoLock();
    const Actor*    GetParent_NoLock() const;


    WorldComponentPtr                GetRootComponent_NoLock();
    ConstWorldComponentPtr           GetRootComponent_NoLock() const;
    std::vector< ComponentPtr >      GetComponents_NoLock();
    std::vector< ConstComponentPtr > GetComponents_NoLock() const;


    virtual void OnTick( float t, float dt ) = 0;

    virtual void TickComponents_NoDelay( float t, float dt );
    virtual void OnPreTickComponents( float t, float dt );
    virtual void OnPostTickComponents( float t, float dt ); // use this to copy Component data to Actor members for caching
                                                            //   if holding the ActorComponent mutex for long is problematic
    virtual void OnSpawned() = 0;
    virtual void OnDespawned() = 0;

    virtual bool OnCreateRenderContext() = 0;
    virtual void OnDestroyRenderContext() = 0;

    //--------------------------------------------------
    // mutexes
    mutable std::mutex mMutActorData;               // protects Actor state data
                                                    //   use this, when modifying Actor state data
                                                    //   note: Actor should only call '_NoLock' functions while holding this mutex
                                                    //         ...although it can send itself messages to be executed on next Tick()
    //--------------------------------------------------
private:
    static std::string GenerateComponentName( const Actor& actor, const std::string& component_name );

    void SetParentPtr_NoLock( Actor* parent );
    void UpdateWorldTransform_NoLock();

    void SetTickEnabled_NoLock( bool value );
    void SetTickRegisteredState_NoLock( bool value );
    void SetTickGroup_NoLock( TickGroup value );
    void SetSpawnedState_NoLock( bool value );

    //--------------------------------------------------
    DoubleBufferedEventQueue mMessageQueue;

    // mutexes
    mutable std::mutex mMutActorMessages;           // protects message registrations, prevents swapping message buffers
                                                    //   use this, when other Actors call this one's public functions to modify its state
    mutable std::mutex mMutActorMessageProcessing;  // protects message execution, prevents swapping message buffers and ticking simultaneously
                                                    //   use this, when Ticking the Actor and processing the arrived messages
    //--------------------------------------------------

    static const std::string mRootComponentName;

    const std::string mName;

    Actor* mParent = nullptr;

    std::vector<ComponentPtr>       mComponents;    // @TODO: optimize (removal is expensive)
    PositionComponentPtr            mRootComponent;

    bool        mRegistered     = false;
    bool        mContextExists  = false;

    bool        mTickEnabled    = true;
    TickGroup   mTickGroup      = TickGroup::NO_GROUP;
    float       mTickInterval   = 0.0f;
    float       mTickLast       = 0.0f;
    bool        mTickRegistered = false;

    bool        mSpawned        = false;


};


} //end of namespace 'engine'

