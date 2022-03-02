/** -----------------------------------------------------------------------------
  * FILE:    ticktester_printercomponent.h
  * AUTHOR:  pttoth - 2018.08.13.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "worldcomponent.h"
#include <string>


class TickTesterPrinterComponent: public engine::WorldComponent{
    std::string _id;

public:
    bool display_tick = false;
    void PrintTick(float t, float dt);

    TickTesterPrinterComponent();
    TickTesterPrinterComponent(const TickTesterPrinterComponent& other) = delete;
    TickTesterPrinterComponent(TickTesterPrinterComponent&& other) = delete;
    virtual ~TickTesterPrinterComponent();
    TickTesterPrinterComponent& operator=(const TickTesterPrinterComponent &other) = delete;
    TickTesterPrinterComponent& operator=(TickTesterPrinterComponent &&other) = delete;
    bool operator==(const TickTesterPrinterComponent &other)const;


public:
    void tick(float t, float dt);
    void OnRegistered();
    void OnUnregistered();

    void setID(char const *id);

protected:
    void onSpawn();
};

