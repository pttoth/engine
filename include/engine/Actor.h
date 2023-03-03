#pragma once

#include "engine/Component.h"
#include "engine/WorldComponent.h"
#include "engine/RealComponent.h"
#include "engine/Message.h"

#include "engine/Common.h"
#include "engine/PositionComponent.h"

#include "pt/event.hpp"

#include <vector>

namespace engine{

enum class TickGroup{
    NO_GROUP = 0, //TODO: probably needs deletion
    PREPHYSICS,
    DURINGPHYSICS,
    POSTPHYSICS,
};

class Actor;
using ActorPtr  = std::shared_ptr< Actor >;
using ActorPtrW = std::weak_ptr< Actor >;


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
        MutexLockGuard lock ( mMutActorMessages );
        mMessageQueue.GetInQueue()->addCallback( message, pt::EventExecRule::TriggerOnce );
    }

    virtual float GetTickInterval() const;
    virtual TickGroup GetTickGroup() const;
    virtual bool IsTickEnabled() const;
    virtual bool IsTickRegistered() const;

    void EnableTick();
    void DisableTick();

    void Spawn();
    void Despawn();
    bool IsSpawned();

    virtual void OnSpawned() = 0;
    virtual void OnDespawned() = 0;

    void AddComponent( Component* component );
    void RemoveComponent( Component* component );

    Actor* GetParent();
    const Actor* GetParent() const;

    static void Tick( Actor& actor, float t, float dt );

    const pt::math::float3    GetPosition() const;
    const pt::math::float4    GetOrientation() const;
    const pt::math::float3    GetScale() const;
    const pt::math::float4x4  GetTransform() const;

    const pt::math::float3    GetWorldPosition() const;
    const pt::math::float4    GetWorldOrientation() const;
    const pt::math::float3    GetWorldScale() const;
    const pt::math::float4x4  GetWorldTransform() const;

    void SetPosition( const pt::math::float3& pos );
    void SetOrientation( const pt::math::float4& orient );
    void SetScale( const pt::math::float3& scale );
    void SetRelativeTransform( const pt::math::float3& pos, const pt::math::float4& orient, const pt::math::float3& scale );

    void SetWorldPosition( const pt::math::float3& pos );
    void SetWorldOrientation( const pt::math::float4& orient );
    void SetWorldScale( const pt::math::float3& scale );
    void SetWorldRelativeTransform( const pt::math::float3& pos, const pt::math::float4& orient, const pt::math::float3& scale );

    void SetParent( Actor& parent );
    void RemoveParent();

protected:
    static void FlushMessages( Actor& actor );

    WorldComponent* GetRootComponent();
    const WorldComponent* GetRootComponent() const;
    std::vector< Component* > GetComponents();
    std::vector< const Component* > GetComponents() const;


    virtual void OnTick( float t, float dt ) = 0;

    virtual void TickComponents( float t, float dt );
    virtual void OnPreTickComponents( float t, float dt );
    virtual void OnPostTickComponents( float t, float dt ); // use this to copy Component data to Actor members for caching
                                                            //   if holding the ActorComponent mutex for long is problematic

    //--------------------------------------------------
    // mutexes
    mutable std::mutex mMutActorComponents;         // protects Actor component data
    //--------------------------------------------------
private:
    static std::string GenerateComponentName( const Actor& actor, const std::string& component_name );

    void SetParentPtr( Actor* parent );
    void UpdateWorldTransform();

    void SetTickEnabledState( bool value );
    void SetTickRegisteredState( bool value );
    void SetTickGroupState( TickGroup value );
    void SetSpawnedState( bool value );

    void CacheRootComponentData() const;
    bool RootComponentIsDirty() const;
    void MarkRootComponentAsDirtyCallback( WorldComponent* wc );
    //--------------------------------------------------
    DoubleBufferedEventQueue mMessageQueue;

    // mutexes
    mutable std::mutex mMutActorMessages;           // protects event registrations, prevents swapping message buffers
    mutable std::mutex mMutActorMessageProcessing;  // protects message execution, prevents swapping message buffers and ticking simultaneously
    mutable std::mutex mMutActorData;               // protects Actor state data
                                                    //  note: Actor should never(!) call outside its context while holding this mutex
    //--------------------------------------------------

    static const std::string mRootComponentName;

    const std::string mName; //TODO: use 'pt::Name' for this

    Actor* mParent = nullptr;

    std::vector<Component*> mComponents;
    PositionComponent mRootComponent;

    mutable bool mRootComponentIsDirty = true;
    mutable pt::math::float3    mCachedPos;
    mutable pt::math::float4    mCachedOrient;
    mutable pt::math::float3    mCachedScale;
    mutable pt::math::float4x4  mCachedTransform;


    bool        mRegistered     = false;

    bool        mTickEnabled    = true;
    TickGroup   mTickGroup      = TickGroup::NO_GROUP;
    float       mTickInterval   = 0.0f;
    float       mTickLast       = 0.0f;
    bool        mTickRegistered = false;

    bool        mSpawned        = false;


};


} //end of namespace engine

