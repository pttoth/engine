/** -----------------------------------------------------------------------------
  * FILE:    playerdisplay.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "physicalcomponent.h"

namespace pttoth {
namespace engine {

class PlayerDisplay: public PhysicalComponent{

public:
    PlayerDisplay();
    PlayerDisplay(const PlayerDisplay& other);
    PlayerDisplay(PlayerDisplay&& other);
    virtual ~PlayerDisplay();
    PlayerDisplay& operator=(const PlayerDisplay &other);
    PlayerDisplay& operator=(PlayerDisplay &&other);
    bool operator==(const PlayerDisplay &other)const;

    // Component interface
public:
    void tick(float t, float dt) override;
    void OnRegistered() override;
    void OnUnregistered() override;

    // WorldComponent interface
protected:
    void onSpawn() override;

    // PhysicalComponent interface
public:
    void OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;
};

} }
