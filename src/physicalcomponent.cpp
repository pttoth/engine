#include "physicalcomponent.h"

using namespace pttoth;
using namespace pttoth::engine;


void PhysicalComponent::
        _RegisterPhysicalComponent(PhysicalComponent *component){
}

void PhysicalComponent::
        _UnregisterPhysicalComponent(PhysicalComponent *component){
}

PhysicalComponent::
        PhysicalComponent(){
}

PhysicalComponent::
        PhysicalComponent(const PhysicalComponent &other){
}

PhysicalComponent::
        PhysicalComponent(PhysicalComponent &&other){
}

PhysicalComponent::
        ~PhysicalComponent(){
}

PhysicalComponent &PhysicalComponent::
        operator=(const PhysicalComponent &other){
}

PhysicalComponent &PhysicalComponent::
        operator=(PhysicalComponent &&other){
}

bool PhysicalComponent::
        operator==(const PhysicalComponent &other) const{
}

void PhysicalComponent::
        tick(float t, float dt){

}

void PhysicalComponent::
        OnRegistered(){

}

void PhysicalComponent::
        OnUnregistered(){

}

void PhysicalComponent::
        onSpawn(){

}


