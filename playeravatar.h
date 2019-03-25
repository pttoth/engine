/** -----------------------------------------------------------------------------
  * FILE:    playeravatar.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "entity.h"
#include "realcomponent.h"

namespace pttoth{

class PlayerAvatar: public engine::Entity{

    engine::RealComponent   _ship;


public:
    PlayerAvatar();
    PlayerAvatar(const PlayerAvatar& other);
    PlayerAvatar(PlayerAvatar&& other);
    virtual ~PlayerAvatar();
    PlayerAvatar& operator=(const PlayerAvatar &other);
    PlayerAvatar& operator=(PlayerAvatar &&other);
    bool operator==(const PlayerAvatar &other)const;




    // Entity interface
protected:
    void tick(float t, float dt) override;

public:
    void OnRegister() override;
    void OnUnregister() override;
};

}
