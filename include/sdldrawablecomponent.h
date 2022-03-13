/** -----------------------------------------------------------------------------
  * FILE:    sdldrawablecomponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component for displaying 2d shapes in SDL.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "DrawableComponent.h"

namespace engine{


class SDLDrawableComponent: public DrawableComponent
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
    //virtual void Draw() = 0;
};


}
