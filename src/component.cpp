#include "component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "worldcomponent.h"
#include "physicalcomponent.h"

#include "services.h"
#include "gamecontrol.h"
#include <assert.h>

using namespace pttoth::engine;

void Component::
        RegisterComponent(Component *component){
    if(nullptr == component){
        throw std::invalid_argument("Component::RegisterComponent() received nullptr as argument");
    }

    WorldComponent* wc = dynamic_cast<PhysicalComponent*>(component);
    PhysicalComponent* pc = dynamic_cast<PhysicalComponent*>(component);

    Component::_RegisterComponent(component);
    if( wc ){
        WorldComponent::_RegisterWorldComponent(wc);
    }
    if( pc ) {
        PhysicalComponent::_RegisterPhysicalComponent(pc);
    }
    component->_registered = true;
}

void Component::
        UnregisterComponent(Component *component){
    PhysicalComponent* pc = dynamic_cast<PhysicalComponent*>(component);
    WorldComponent* wc = dynamic_cast<PhysicalComponent*>(component);
    if( pc ){
        PhysicalComponent::_UnregisterPhysicalComponent(pc);
    }
    if( wc ) {
        WorldComponent::_UnregisterWorldComponent(wc);
    }
    Component::_UnregisterComponent(component);
    component->_registered = false;
}

void Component::
        makeDirty(){
    _is_dirty = true;
}

bool Component::
        isDirty() const{
    return _is_dirty;
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
        _RegisterComponent(Component *component){
    if( !component->isRegistered() ){
        Services::getGameControl()->registerComponent(component);
    }else{
        assert(false);
    }
}

void Component::
        _UnregisterComponent(Component *component){
    if( component->isRegistered() ){
        Services::getGameControl()->unregisterComponent(component);
    }else{
        assert(false);
    }
}

void Component::
        cleanDirty(){
    _is_dirty = false;
}

Component::
        Component():_tick_enabled(false),
                    _registered(false){
}

Component::
        ~Component(){
}


