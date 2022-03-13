#include "component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "worldcomponent.h"
#include "realcomponent.h"

#include "services.h"
#include "enginecontrol.h"
#include <assert.h>

using namespace engine;

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
        Services::getGameControl()->registerComponent(component);
    }else{
        assert(false);
    }
}


void Component::
UnregisterComponentParts(Component *component)
{
    if( component->isRegistered() ){
        Services::getGameControl()->unregisterComponent(component);
    }else{
        assert(false);
    }
}


Component::
Component():mTickEnabled(false),
            mIsRegistered(false)
{}


Component::
~Component()
{}


