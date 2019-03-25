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

#include "playercontroller.h"
#include "projectilesystem.h"

namespace pttoth{

class RaptorGame2D: public engine::Game{

    enum CfgKey{
        cKeyMoveForward = 1,
        cKeyMoveBackward,
        cKeyMoveLeft,
        cKeyMoveRight,
        cKeyFire,
        cKeySpecialFire,

        fPlayerSpeed,
    };

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
    void _setDefaultConfigValues();
    void _setConfigFromLocalVariables();    //throws std::invalid_argument
    void _setLocalVariablesFromConfig();
    void _playerMoveForward(float t, float dt);
    void _playerMoveBackward(float t, float dt);
    void _playerMoveLeft(float t, float dt);
    void _playerMoveRight(float t, float dt);
    void _playerFireMain(float t, float dt);
    void _playerFireSpecial(float t, float dt);

    int count;

    SDL_Window*         _window;
    SDL_Renderer*       _renderer;

    PlayerController    _playercontroller;

    std::string         _config_filename;
    std::string         _config_default_filename;
    Config              _config_raptorgame;
    math::float2        _playerpos;
    ProjectileSystem    _projsys;
};

}
