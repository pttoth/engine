/** -----------------------------------------------------------------------------
  * FILE:    SDLControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Wrapper class for accessing SDL functionality.
  * TODO:    Make it thread-safe.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <mutex>

#include "SDL2/SDL.h"

#include "pt/math.h"

struct SDL_Window;
struct SDL_Renderer;

namespace engine{

class SDLControl{

public:
    virtual ~SDLControl(){}

    virtual SDL_Window*     GetMainWindow() = 0;
    virtual SDL_Renderer*   GetMainRenderer() = 0;
    virtual void            SetMainWindow( SDL_Window* window ) = 0;
    virtual void            SetMainRenderer( SDL_Renderer* renderer ) = 0;

    virtual uint32_t        GetMainWindowWidth() const = 0;
    virtual uint32_t        GetMainWindowHeight() const = 0;
    virtual void            SetMainWindowWidth( uint32_t width ) = 0;
    virtual void            SetMainWindowHeight( uint32_t height ) = 0;

protected:

private:

};

} // end of namespace 'engine'
