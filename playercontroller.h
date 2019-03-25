/** -----------------------------------------------------------------------------
  * FILE:    playercontroller.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "entity.h"
#include "playeravatar.h"
#include "SDL2/SDL.h"

#include "pttoth/event.hpp"

namespace pttoth{

class PlayerController: public engine::Entity{

public:
    enum class PlayerAction{
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_LEFT,
        MOVE_RIGHT,
        FIRE_MAIN,
        FIRE_SPECIAL,
    };

    //player action callbacks
    //      parameters are the same as tick(t,dt)
    event<float, float> evMoveForward;
    event<float, float> evMoveBackward;
    event<float, float> evMoveLeft;
    event<float, float> evMoveRight;
    event<float, float> evFireMain;
    event<float, float> evFireSpecial;

    //ctors
    PlayerController();
    PlayerController(const PlayerController& other);
    PlayerController(PlayerController&& other);
    virtual ~PlayerController();
    PlayerController& operator=(const PlayerController &other);
    PlayerController& operator=(PlayerController &&other);
    bool operator==(const PlayerController &other)const;

    void setAvatar(PlayerAvatar* avatar);

    void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
    void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);

    void setKey(PlayerAction action, char key);
    char getKey(PlayerAction action) const;

private:
    PlayerAvatar*   _avatar;

    //key states
    bool    _keydownMoveForward;
    bool    _keydownMoveBackward;
    bool    _keydownMoveLeft;
    bool    _keydownMoveRight;
    bool    _keydownFireMain;
    bool    _keydownFireSpecial;

    //key mapping
    char    _keyMoveForward;
    char    _keyMoveBackward;
    char    _keyMoveLeft;
    char    _keyMoveRight;
    char    _keyFireMain;
    char    _keyFireSpecial;

    // Entity interface
protected:
    void tick(float t, float dt) override;

public:
    void OnRegister() override;
    void OnUnregister() override;
};

}

