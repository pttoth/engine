#include "engine/test/ticktester/PrinterEntity.h"

#include <iostream>
#include <assert.h>

void TickTesterPrinterEntity::
printTick(float t, float dt) const
{
    if(mDisplayTick){
        std::cout << mId << " tick(" << t << " ; " << dt<< ")\n";
    }
}


TickTesterPrinterEntity::
TickTesterPrinterEntity(const std::string& name):
    Entity(name)
{}


TickTesterPrinterEntity::
TickTesterPrinterEntity(const TickTesterPrinterEntity& other):
    Entity(other)
{}


TickTesterPrinterEntity::
~TickTesterPrinterEntity()
{}


bool TickTesterPrinterEntity::
operator==(const TickTesterPrinterEntity &other) const
{
    assert(false); //TODO: implement
    return false;
}


void TickTesterPrinterEntity::
Tick(float t, float dt)
{
    printTick(t,dt);
}


void TickTesterPrinterEntity::
OnRegister()
{
    std::cout << mId << " registered\n";
}


void TickTesterPrinterEntity::
OnUnregister()
{
    std::cout << mId << " unregistered\n";
}


void TickTesterPrinterEntity::
setID(const char *id)
{
    mId = id;
}
