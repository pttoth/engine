#include "Component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "WorldComponent.h"
#include "RealComponent.h"

#include "Services.h"
#include "EngineControl.h"
#include <assert.h>

#include <sstream>

using namespace engine;

std::string
GenerateComponentName(const std::string& sourcename)
{
    std::stringstream ss;
    //TODO: cross-reference name with existing name pool
    //      find a new, unused name for the entity
    //      (remove numbers from the end and append a new number instead)

    //TODO: remove this temp solution
    if( 0 == sourcename.length() ){
        ss << "AutoGenName"; //TODO: use a cross-referenced free number instead
    }else{
        ss << sourcename << "_copy";
    }
    return ss.str();
}


Component::
Component(const std::string& name):
    mName(name),
    mTickEnabled(false),
    mIsRegistered(false)
{}


Component::
Component(const Component& other):
    Component( GenerateComponentName( other.GetName() ) )
{}


Component::
~Component()
{}


void Component::
RegisterComponent(Component *component)
{
    if(nullptr == component){
        throw std::invalid_argument("Component::RegisterComponent() received nullptr as argument");
    }

    //a note on the bad practice below:
    //  We only want WorldComponents and RealComponents redefining the registration scheme,
    //    afaik this restriction in pre-c++11 versions could only be done this way.
    //  Post-c++11 could use 2 'private' classes: BaseComponent and BaseWorldComponent
    //    which would allow redefinintions, the 3 Components would inherit from those and then
    //    the three 'public' Components would use the final keyword for their register functions.
    //  I'm not gonne make extra 2 classes and the full overhead just because of this.
    //    These 3 classes exist in close relation to each other, so Component can afford to know
    //    its two immediate children for this one purpose, despite the truly bad practice.

    WorldComponent* wc = dynamic_cast<WorldComponent*>(component);
    RealComponent* rc = dynamic_cast<RealComponent*>(component);

    Component::RegisterComponentParts(component);
    if( wc != nullptr ){
        WorldComponent::RegisterWorldComponentParts(wc);
    }
    if( rc != nullptr ) {
        RealComponent::RegisterRealComponentParts(rc);
    }
    component->mIsRegistered = true;
}


void Component::
UnregisterComponent(Component *component)
{
    RealComponent* rc = dynamic_cast<RealComponent*>(component);
    WorldComponent* wc = dynamic_cast<WorldComponent*>(component);
    if( rc != nullptr ){
        RealComponent::UnregisterRealComponentParts(rc);
    }
    if( wc != nullptr ){
        WorldComponent::UnregisterWorldComponentParts(wc);
    }
    Component::UnregisterComponentParts(component);
    component->mIsRegistered = false;
}


void Component::
OnAddedToEntity(entity::ComponentVisitor&)
{}


void Component::
OnRemovedFromEntity(entity::ComponentVisitor&)
{}


void Component::
Spawn()
{}


void Component::
Despawn()
{}


const std::string& Component::
GetName() const
{
    return mName;
}


void Component::
enableTick()
{
    mTickEnabled = true;
}


void Component::
disableTick()
{
    mTickEnabled = false;
}


bool Component::
isRegistered()
{
    return mIsRegistered;
}


void Component::
RegisterComponentParts(Component *component)
{
    if( !component->isRegistered() ){
        Services::GetEngineControl()->RegisterComponent( *component );
    }else{
        assert(false);
    }
}


void Component::
UnregisterComponentParts(Component *component)
{
    if( component->isRegistered() ){
        Services::GetEngineControl()->UnregisterComponent( *component );
    }else{
        assert(false);
    }
}
