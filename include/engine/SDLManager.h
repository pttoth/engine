/** -----------------------------------------------------------------------------
  * FILE:    SDLManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Wrapper class for accessing SDL functionality.
  * TODO:    Make it thread-safe.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/SDLControl.h"

#include "pt/math.h"
#include "SDL2/SDL.h"
#include <mutex>

struct SDL_Window;
struct SDL_Renderer;

namespace engine{

class SDLManager: public SDLControl{

public:
    SDLManager();
    SDLManager( const SDLManager& other ) = delete;
    SDLManager( SDLManager&& other ) = delete;
    virtual ~SDLManager();
    SDLManager& operator=( const SDLManager& other ) = delete;
    SDLManager& operator=( SDLManager&& other ) = delete;
    bool operator==( const SDLManager& other )const = delete;

    SDL_Window*     GetMainWindow() override;
    SDL_Renderer*   GetMainRenderer() override;
    void            SetMainWindow( SDL_Window* window ) override;
    void            SetMainRenderer( SDL_Renderer* renderer ) override;

    uint32_t        GetMainWindowWidth() const override;
    uint32_t        GetMainWindowHeight() const override;
    void            SetMainWindowWidth( uint32_t width ) override;
    void            SetMainWindowHeight( uint32_t height ) override;


protected:

private:

    uint32_t        mMainWindowWidth;
    uint32_t        mMainWindowHeight;

    SDL_Window*     mMainWindow;
    SDL_Renderer*   mMainRenderer;
};

} // end of namespace 'engine'
