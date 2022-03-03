/** -----------------------------------------------------------------------------
  * FILE:    sdldrawablecomponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component for displaying 2d shapes in SDL.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "realcomponent.h"

namespace engine{


class SDLDrawableComponent: public RealComponent
{
public:
    SDLDrawableComponent(){}
    SDLDrawableComponent(const SDLDrawableComponent& other) = delete;
    SDLDrawableComponent(SDLDrawableComponent&& other) = delete;
    virtual ~SDLDrawableComponent(){}
    SDLDrawableComponent& operator=(const SDLDrawableComponent &other) = delete;
    SDLDrawableComponent& operator=(SDLDrawableComponent &&other) = delete;
    bool operator==(const SDLDrawableComponent &other);

public:
    void tick(float t, float dt) = 0;// override;
    void OnRegistered() = 0;// override;
    void OnUnregistered() = 0;// override;

    virtual void draw() = 0;

};


}

