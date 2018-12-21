/** -----------------------------------------------------------------------------
  * FILE:    progkonfshowcasegame.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Shows off some unfinished/basic functionality of the engine.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "game.h"
#include "player.h"

namespace pttoth {
namespace engine {

class ProgKonfShowcaseGame: public pttoth::engine::Game{
    Player*         _player;
    math::float3    _startpos_player;

    void StartGame();
public:
    ProgKonfShowcaseGame();
    virtual ~ProgKonfShowcaseGame();

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

} }

