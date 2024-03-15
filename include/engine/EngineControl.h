/** -----------------------------------------------------------------------------
  * FILE:    EngineControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "SDL2/SDL.h"
#include <cstdint>

namespace engine{

class EngineControl
{
public:
    virtual bool        DeveloperMode() const = 0;
    virtual uint32_t    GetCurrentTime() const = 0;
    virtual SDL_Window* GetMainWindow() = 0;

protected:
private:
};

} //end of namespace 'engine'
