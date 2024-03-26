#include "engine/component/Component.h"

#include "pt/logging.h"
#include <assert.h>
#include <sstream>

using namespace engine;


std::string Component::
GenerateComponentName( const std::string& parentname, const std::string& componentname )
{
    assert( 0 < componentname.length() );

    std::stringstream ss;
    //TODO: cross-reference name with existing name pool
    //      find a new, unused name for the actor
    //      (remove numbers from the end and append a new number instead)

    ss << parentname << "." << componentname;
    return ss.str();
}


//--------------------------------------------------
//  Component
//--------------------------------------------------

Component::
Component( const std::string &name ):
    mName( name )
{}


Component::
Component( Component&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


Component::
~Component()
{}


Component& Component::
operator=( Component&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
    return *this;
}


const std::string& Component::
GetName() const
{
    return mName;
}


void Component::
Spawn()
{
    if( !IsSpawned() ){
        PT_LOG_DEBUG( "Spawning component '" << this->GetName() << "'" );
        mSpawned = true;
        OnSpawned();
    }
}


void Component::
Despawn()
{
    if( IsSpawned() ){
        PT_LOG_DEBUG( "Despawning component '" << this->GetName() << "'" );
        mSpawned = false;
        OnDespawned();
    }
}


bool Component::
IsSpawned() const
{
    return mSpawned;
}


void Component::
Tick( float t, float dt )
{
    OnTick( t, dt );
}
