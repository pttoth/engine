/** -----------------------------------------------------------------------------
  * FILE:    raptorgame2d.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "game.h"
#include "SDL2/SDL.h"
#include "math/float2.h"

#include "pttoth/config.h"

#include "projectilesystem.h"

namespace pttoth{

class RaptorGame2D: public engine::Game{

public:
    RaptorGame2D();
    RaptorGame2D(const RaptorGame2D& other);
    RaptorGame2D(RaptorGame2D&& other);
    virtual ~RaptorGame2D();
    RaptorGame2D& operator=(const RaptorGame2D &other);
    RaptorGame2D& operator=(RaptorGame2D &&other);
    bool operator==(const RaptorGame2D &other)const;

protected:
    void onStart() override;
    void onExit() override;
    void onShutdownSignal() override;
    void tick(float t, float dt) override;
    void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void drawScene(float t, float dt) override;
private:
    SDL_Window*         _window;
    SDL_Renderer*       _renderer;


    Config              _config_raptorgame;
    math::float2        _playerpos;
    ProjectileSystem    _projsys;
};

}
