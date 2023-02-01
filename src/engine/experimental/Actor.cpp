#include "engine/experimental/Actor.h"

using namespace engine;
using namespace experimental;


Actor::DoubleBufferedEventQueue::
DoubleBufferedEventQueue():
    mMessageQueue1( mTrigger1 ),
    mMessageQueue2( mTrigger2 )
{}


Actor::DoubleBufferedEventQueue::
~DoubleBufferedEventQueue()
{}


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
Actor()
{}


Actor::
Actor( const Actor& other )
{}


Actor::
Actor( Actor&& source )
{}


Actor::
~Actor()
{}


Actor &Actor::
operator=( const Actor& other )
{}


Actor &Actor::
operator=( Actor&& source )
{}


bool Actor::
operator==( const Actor& other ) const
{}


const std::string &Actor::GetName() const
{

}


//--------------------------------------------------

Actor *Actor::
GetParent()
{
    return mParent;
}


void Actor::Tick()
{}

void Actor::PostMessage(Message msg)
{}

void Actor::FlushMessages()
{

}

void Actor::TickComponents()
{}

void Actor::SetParent()
{}

