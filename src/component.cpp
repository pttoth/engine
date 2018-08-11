#include "component.h"

//required during registration
//  knows these two children classes,
//  because all three are basic engine functionality
#include "worldcomponent.h"
#include "physicalcomponent.h"

using namespace pttoth::engine;

void Component::
        RegisterComponent(Component *component){
    PhysicalComponent* pc = dynamic_cast<PhysicalComponent*>(component);
    WorldComponent* wc = dynamic_cast<PhysicalComponent*>(component);
    if( pc ){ //component is PhysicalComponent
        Component::_RegisterComponent(component);
    }else if( wc ) { //component is WorldComponent
        WorldComponent::_RegisterWorldComponent(wc);
    }else{ //component is Component
        PhysicalComponent::_RegisterPhysicalComponent(pc);
    }
    component->_registered = true;
    component->OnRegistered();
}

void Component::
        UnregisterComponent(Component *component){
    PhysicalComponent* pc = dynamic_cast<PhysicalComponent*>(component);
    WorldComponent* wc = dynamic_cast<PhysicalComponent*>(component);
    if( pc ){ //component is PhysicalComponent
        Component::_UnregisterComponent(component);
    }else if( wc ) { //component is WorldComponent
        WorldComponent::_UnregisterWorldComponent(wc);
    }else{ //component is Component
        PhysicalComponent::_UnregisterPhysicalComponent(pc);
    }
    component->_registered = false;
    component->OnUnregistered();
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

}

void Component::
        _UnregisterComponent(Component *component){

}

Component::
        Component():_tick_enabled(false),
                    _registered(false){
}

Component::
        ~Component(){
}


