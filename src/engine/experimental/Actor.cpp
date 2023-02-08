#include "engine/experimental/Actor.h"

#include "engine/experimental/ComponentVisitor.h"

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


void Actor::AddedWorldComponent(WorldComponent *component)
{
    assert( false );
}


void Actor::RemovedWorldComponent(WorldComponent *component)
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
    mMessagesTrigger(),
    mMessages( mMessagesTrigger ),
    mName( name ),
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
{}


Actor::
Actor( const Actor& other ):
    //TODO: see, whether Event will have to be reconstructed here or copied
    mMessagesTrigger(),
    mMessages( mMessagesTrigger ),
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
{}


Actor::
Actor( Actor&& source ):
    //TODO: Event probably has to be moved instead of reconstructed here
    mMessagesTrigger(),
    mMessages( mMessagesTrigger ),
    mRootComponent( Actor::GenerateComponentName( *this, mRootComponentName ) )
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


const std::string &Actor::
GetName() const
{
    return mName;
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
            mMessages.addCallback( lambda, EventExecRule::TriggerOnce );
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
            mMessages.addCallback( lambda, EventExecRule::TriggerOnce );
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
Tick()
{
    assert( false );
}


void Actor::
FlushMessages()
{
    assert( false );
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
