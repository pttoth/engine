/** -----------------------------------------------------------------------------
  * FILE:    ticktester_printerentity.h
  * AUTHOR:  pttoth - 2018.08.13.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "entity.h"

#include <string>


class TickTesterPrinterEntity: public engine::Entity{
    std::string _id;
    unsigned testnum = 0;


public:
    bool display_tick = false;
    void printTick(float t, float dt) const;

    TickTesterPrinterEntity();
    TickTesterPrinterEntity(const TickTesterPrinterEntity& other) = delete;
    TickTesterPrinterEntity(TickTesterPrinterEntity&& other) = delete;
    virtual ~TickTesterPrinterEntity();
    TickTesterPrinterEntity& operator=(const TickTesterPrinterEntity &other) = delete;
    TickTesterPrinterEntity& operator=(TickTesterPrinterEntity &&other) = delete;
    bool operator==(const TickTesterPrinterEntity &other)const;

    // Entity interface
protected:
    void tick(float t, float dt);

public:
    void OnRegister();
    void OnUnregister();

    void setID(char const *id);

};


