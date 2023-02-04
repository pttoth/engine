#include "engine/experimental/Component.h"

#include "engine/experimental/ComponentVisitor.h"

#include <sstream>
#include <assert.h>

using namespace engine;
using namespace experimental;


std::string Component::
GenerateComponentName( const std::string& parentname, const std::string& componentname )
{
    assert( 0 < componentname.length() );

    std::stringstream ss;
    //TODO: cross-reference name with existing name pool
    //      find a new, unused name for the entity
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
Component( const Component &other ):
    mName( GenerateComponentName( other.GetName(), "copy" ) )
{
    assert( false );
}


Component::
Component( Component &&source )
{
    assert( false );
}


Component::
~Component()
{}


const std::string &Component::
GetName() const
{
    return mName;
}


void Component::
OnAddedToEntity( ComponentVisitor &visitor )
{}


void Component::
OnRemovedFromEntity( ComponentVisitor &visitor )
{}


void Component::
Spawn()
{}


void Component::
Despawn()
{}
