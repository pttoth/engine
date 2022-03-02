/** -----------------------------------------------------------------------------
  * FILE:    ticktester_game.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine.h"
#include "test/ticktester_printercomponent.h"
#include "test/ticktester_printerentity.h"



class TickTesterGame: public engine::Engine{
    TickTesterPrinterEntity emouse;
    TickTesterPrinterEntity ekeyboard;

    TickTesterPrinterEntity eDependencyTester_sub;
    TickTesterPrinterEntity eDependencyTester_dep;

    TickTesterPrinterComponent eDepTesterCompSub1;
    TickTesterPrinterComponent eDepTesterCompSub2;
    TickTesterPrinterComponent eDepTesterCompDep1;
    TickTesterPrinterComponent eDepTesterCompDep2;

public:
    TickTesterGame();
    virtual ~TickTesterGame();

    // Application interface
protected:
    virtual void onStart() override;
    virtual void onExit() override;
    virtual void onShutdownSignal() override;

    // Game interface
protected:
    virtual void tick(float t, float dt) override;
    virtual void onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    virtual void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    virtual void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
};

