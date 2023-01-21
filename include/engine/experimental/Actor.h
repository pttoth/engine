#pragma once

#include "engine/experimental/Component.h"
#include "engine/experimental/Message.h"

#include "pt/event.hpp"

#include <mutex>
#include <shared_mutex>
//#include <thread>
#include <vector>

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

    Actor* GetParent();

    virtual void Tick();
    virtual void PostMessage( Message msg );


protected:
    virtual void TickComponents();

    virtual void SetParent();

private:
    Actor* mParent = nullptr;

    std::vector<Component*> mComponents;


    std::mutex mutex_event_input;
    std::mutex mutex_event_processing;


    //std::vector<Message>    mMessages;


    mutable std::shared_timed_mutex mMutex;
};


