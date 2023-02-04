#pragma once

#include "engine/experimental/Component.h"
#include "engine/experimental/WorldComponent.h"
#include "engine/experimental/RealComponent.h"
#include "engine/experimental/Message.h"

#include "engine/Common.h"
#include "engine/experimental/PositionComponent.h"

#include "pt/event.hpp"

#include <vector>

namespace engine{
namespace experimental{

class Actor;
using ActorPtr  = std::shared_ptr< Actor >;
using ActorPtrW = std::weak_ptr< Actor >;


class Actor
{
    friend class ComponentVisitor;

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

        pt::Event<>* GetInQueue();
        pt::Event<>* GetProcQueue();
        void SwapBuffers();
    };
    // --------------------------------------------------

public:
    Actor();
    Actor( const Actor& other );
    Actor( Actor&& source );

    virtual ~Actor();

    Actor& operator=( const Actor& other );
    Actor& operator=( Actor&& source );

    bool operator==( const Actor& other ) const;

    //-----

    const std::string& GetName() const;

    void AddComponent( Component* component );
    void RemoveComponent( Component* component );
    //std::vector< Component* > GetComponents(); //make this protected?
    WorldComponent* GetRootComponent();

    Actor* GetParent();

    virtual void Tick();


protected:
    static void FlushMessages();


    virtual void TickComponents();

    virtual void SetParent();
    virtual void RemoveParent();

    DoubleBufferedEventQueue mEventQueues;

    template<class TMut>
    std::unique_lock<TMut> GetLock( TMut& m ){
        return std::unique_lock<TMut> ( m );
    }

private:
    static std::string GenerateComponentName( const Actor& actor, const std::string& component_name );

    void AddedWorldComponent( WorldComponent* component );
    void RemovedWorldComponent( WorldComponent* component );

    //--------------------------------------------------
    pt::EventTrigger<>      mMessagesTrigger;
    pt::Event<>             mMessages;

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





    //std::vector<Message>    mMessages;



};


} //end of namespace experimental
} //end of namespace engine

