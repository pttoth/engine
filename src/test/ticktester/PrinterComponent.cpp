#include "test/ticktester/PrinterComponent.h"

#include <iostream>
#include <assert.h>

void TickTesterPrinterComponent::
PrintTick(float t, float dt)
{
    if(mDisplayTick){
        std::cout << mId << " tick(" << t << " ;" << dt<< ")\n";
    }
}


TickTesterPrinterComponent::
TickTesterPrinterComponent(const std::string& name):
    WorldComponent(name)
{}


TickTesterPrinterComponent::
TickTesterPrinterComponent(const TickTesterPrinterComponent& other):
    WorldComponent(other)
{}


TickTesterPrinterComponent::
~TickTesterPrinterComponent()
{}


bool TickTesterPrinterComponent::
operator==(const TickTesterPrinterComponent &other) const
{
    assert(false); //TODO: implement
    return false;
}


void TickTesterPrinterComponent::
tick(float t, float dt)
{
    PrintTick(t,dt);
}


void TickTesterPrinterComponent::
OnRegistered()
{
    std::cout << mId << " registered\n";
}


void TickTesterPrinterComponent::
OnUnregistered()
{
    std::cout << mId << " unregistered\n";
}


void TickTesterPrinterComponent::
setID(const char *id)
{
    mId = id;
}


void TickTesterPrinterComponent::
onSpawn()
{
    std::cout << mId << " spawned\n";
}
