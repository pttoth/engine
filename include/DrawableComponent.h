/** -----------------------------------------------------------------------------
  * FILE:    DrawableComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Base component for anything with a visual representation.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "realcomponent.h"

namespace engine{


class DrawableComponent: public RealComponent
{
public:
    DrawableComponent(){}
    DrawableComponent(const DrawableComponent& other) = delete;
    DrawableComponent(DrawableComponent&& other) = delete;
    virtual ~DrawableComponent(){}
    DrawableComponent& operator=(const DrawableComponent &other) = delete;
    DrawableComponent& operator=(DrawableComponent &&other) = delete;
    bool operator==(const DrawableComponent &other);

public:
    virtual void Draw() = 0;
};


}

