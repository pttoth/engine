#include "test/ticktester_printercomponent.h"

#include <iostream>
#include <assert.h>

void TickTesterPrinterComponent::
        PrintTick(float t, float dt){
    if(display_tick){
        std::cout << _id << " tick(" << t << " ;" << dt<< ")\n";
    }
}

TickTesterPrinterComponent::
        TickTesterPrinterComponent(){
}

TickTesterPrinterComponent::
        ~TickTesterPrinterComponent(){
}

bool TickTesterPrinterComponent::
        operator==(const TickTesterPrinterComponent &other) const{
    assert(false); //TODO: implement
    return false;
}

void TickTesterPrinterComponent::
        tick(float t, float dt){
    PrintTick(t,dt);
}

void TickTesterPrinterComponent::
        OnRegistered(){
    std::cout << _id << " registered\n";
}

void TickTesterPrinterComponent::
        OnUnregistered(){
    std::cout << _id << " unregistered\n";
}

void TickTesterPrinterComponent::
        setID(const char *id){
    _id = id;
}

void TickTesterPrinterComponent::
        onSpawn(){
    std::cout << _id << " spawned\n";
}
