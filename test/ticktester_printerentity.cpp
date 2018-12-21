#include "test/ticktester_printerentity.h"

#include <iostream>

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
        TickTesterPrinterEntity(const TickTesterPrinterEntity &other){
}

TickTesterPrinterEntity::
        TickTesterPrinterEntity(TickTesterPrinterEntity &&other){
}

TickTesterPrinterEntity::
        ~TickTesterPrinterEntity(){
}

TickTesterPrinterEntity &TickTesterPrinterEntity::
        operator=(const TickTesterPrinterEntity &other){
}

TickTesterPrinterEntity &TickTesterPrinterEntity::
        operator=(TickTesterPrinterEntity &&other){
}

bool TickTesterPrinterEntity::
        operator==(const TickTesterPrinterEntity &other) const{
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

void TickTesterPrinterEntity::OnSpawn()
{

}
