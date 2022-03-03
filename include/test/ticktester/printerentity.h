#pragma once

#include "entity.h"

#include <string>


class TickTesterPrinterEntity: public engine::Entity{
    std::string mId;
    unsigned mTestnum = 0;


public:
    bool mDisplayTick = false;
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


