#include "engine/Component.h"

#include <sstream>
#include <assert.h>

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
~Component()
{}


const std::string &Component::
GetName() const
{
    return mName;
}


void Component::
Spawn()
{
    OnSpawned();
}


void Component::
Despawn()
{
    OnDespawned();
}


void Component::
Tick( float t, float dt )
{
    OnTick( t, dt );
}
