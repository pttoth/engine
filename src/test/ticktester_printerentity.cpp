#include "test/ticktester_printerentity.h"

#include <iostream>
#include <assert.h>

void TickTesterPrinterEntity::printTick(float t, float dt) const
{
    if(display_tick){
        std::cout << _id << " tick(" << t << " ; " << dt<< ")\n";
    }
}

TickTesterPrinterEntity::
        TickTesterPrinterEntity(){
}

TickTesterPrinterEntity::
        ~TickTesterPrinterEntity(){
}

bool TickTesterPrinterEntity::
        operator==(const TickTesterPrinterEntity &other) const{
    assert(false); //TODO: implement
    return false;
}

void TickTesterPrinterEntity::
        tick(float t, float dt){
    printTick(t,dt);
}

void TickTesterPrinterEntity::
        OnRegister(){
    std::cout << _id << " registered\n";
}

void TickTesterPrinterEntity::
        OnUnregister(){
    std::cout << _id << " unregistered\n";
}

void TickTesterPrinterEntity::
        setID(const char *id){
    _id = id;
}
