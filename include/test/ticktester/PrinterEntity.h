#pragma once

#include "Entity.h"

#include <string>


class TickTesterPrinterEntity: public engine::Entity{
    std::string mId;
    unsigned mTestnum = 0;


public:
    bool mDisplayTick = false;
    void printTick(float t, float dt) const;

    TickTesterPrinterEntity(const std::string& name);
    TickTesterPrinterEntity(const TickTesterPrinterEntity& other);
    TickTesterPrinterEntity(TickTesterPrinterEntity&& other) = delete;
    virtual ~TickTesterPrinterEntity();
    TickTesterPrinterEntity& operator=(const TickTesterPrinterEntity &other) = delete;
    TickTesterPrinterEntity& operator=(TickTesterPrinterEntity &&other) = delete;
    bool operator==(const TickTesterPrinterEntity &other)const;

    // Entity interface
protected:
    void Tick(float t, float dt);

public:
    void OnRegister();
    void OnUnregister();

    void setID(char const *id);

};


