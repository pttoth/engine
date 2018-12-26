#include "component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "worldcomponent.h"
#include "realcomponent.h"

#include "services.h"
#include "gamecontrol.h"
#include <assert.h>

using namespace pttoth::engine;

void Component::
        RegisterComponent(Component *component){
    if(nullptr == component){
        throw std::invalid_argument("Component::RegisterComponent() received nullptr as argument");
    }

    WorldComponent* wc = dynamic_cast<WorldComponent*>(component);
    RealComponent* rc = dynamic_cast<RealComponent*>(component);

    Component::_RegisterComponentParts(component);
    if( wc ){
        WorldComponent::_RegisterWorldComponentParts(wc);
    }
    if( rc ) {
        RealComponent::_RegisterRealComponentParts(rc);
    }
    component->_registered = true;
}

void Component::
        UnregisterComponent(Component *component){
    RealComponent* rc = dynamic_cast<RealComponent*>(component);
    WorldComponent* wc = dynamic_cast<WorldComponent*>(component);
    if( rc ){
        RealComponent::_UnregisterRealComponentParts(rc);
    }
    if( wc ) {
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


