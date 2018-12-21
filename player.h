/** -----------------------------------------------------------------------------
  * FILE:    player.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once


#include "entity.h"

#include "SDL2/SDL.h"

#include "physicalcomponent.h"
#include "playerdisplay.h"

namespace pttoth{
namespace engine{


class Player: public Entity{

    math::float3 dir;
    float move_speed = 2.0f;
    PlayerDisplay _displaycomponent;

public:
    Player();
    Player(const Player& other);
    Player(Player&& other);
    virtual ~Player();
    Player& operator=(const Player &other);
    Player& operator=(Player &&other);
    bool operator==(const Player &other)const;

    void onKeyDown(SDL_Keycode keycode, uint16_t keymod,
                   uint32_t timestamp, uint8_t repeat);

    void onKeyUp(SDL_Keycode keycode, uint16_t keymod,
                 uint32_t timestamp, uint8_t repeat);

    // Entity interface
protected:
    void tick(float t, float dt) override;

public:
    void OnRegister() override;
    void OnUnregister() override;
    void OnSpawn() override;
};

} }
