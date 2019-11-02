#include "component.h"

//required during registration (bad OOP design)
//  knows these two children classes,
//  because all three are basic engine functionality
#include "worldcomponent.h"
#include "realcomponent.h"

#include "services.h"
#include "gamecontrol.h"
#include <assert.h>

using namespace PT;
using namespace PT::engine;

void Component::
        RegisterComponent(Component *component){
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

    Component::_RegisterComponentParts(component);
    if( wc != nullptr ){
        WorldComponent::_RegisterWorldComponentParts(wc);
    }
    if( rc != nullptr ) {
        RealComponent::_RegisterRealComponentParts(rc);
    }
    component->_registered = true;
}

void Component::
        UnregisterComponent(Component *component){
    RealComponent* rc = dynamic_cast<RealComponent*>(component);
    WorldComponent* wc = dynamic_cast<WorldComponent*>(component);
    if( rc != nullptr ){
        RealComponent::_UnregisterRealComponentParts(rc);
    }
    if( wc != nullptr ){
        WorldComponent::_UnregisterWorldComponentParts(wc);
    }
    Component::_UnregisterComponentParts(component);
    component->_registered = false;
}

void Component::
        enableTick(){
    _tick_enabled = true;
}

void Component::
        disableTick(){
    _tick_enabled = false;
}

bool Component::
        isRegistered(){
    return _registered;
}

void Component::
        _RegisterComponentParts(Component *component){
    if( !component->isRegistered() ){
        Services::getGameControl()->registerComponent(component);
    }else{
        assert(false);
    }
}

void Component::
        _UnregisterComponentParts(Component *component){
    if( component->isRegistered() ){
        Services::getGameControl()->unregisterComponent(component);
    }else{
        assert(false);
    }
}

Component::
        Component():_tick_enabled(false),
                    _registered(false){
}

Component::
        ~Component(){
}


