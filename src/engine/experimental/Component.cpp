#include "engine/experimental/Component.h"

#include "engine/experimental/ComponentVisitor.h"


using namespace engine;
using namespace experimental;



//--------------------------------------------------
//  Component
//--------------------------------------------------

Component::
Component( const std::string &name )
{}


Component::
~Component()
{}


void Component::
OnAddedToEntity( ComponentVisitor &visitor )
{}


void Component::
OnRemovedFromEntity( ComponentVisitor &visitor )
{}


const std::string &Component::
GetName() const
{
    return mName;
}
