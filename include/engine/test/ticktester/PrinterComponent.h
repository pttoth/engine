#pragma once

#include "engine/WorldComponent.h"

#include <string>

class TickTesterPrinterComponent: public engine::WorldComponent{
    std::string mId;

public:
    bool mDisplayTick = false;
    void PrintTick(float t, float dt);

    TickTesterPrinterComponent(const std::string& name);
    TickTesterPrinterComponent(const TickTesterPrinterComponent& other);
    TickTesterPrinterComponent(TickTesterPrinterComponent&& other) = delete;
    virtual ~TickTesterPrinterComponent();
    TickTesterPrinterComponent& operator=(const TickTesterPrinterComponent &other) = delete;
    TickTesterPrinterComponent& operator=(TickTesterPrinterComponent &&other) = delete;
    bool operator==(const TickTesterPrinterComponent &other)const;


public:
    void Tick(float t, float dt);
    void OnRegistered();
    void OnUnregistered();

    void setID(char const *id);

protected:
    void onSpawn();
};

