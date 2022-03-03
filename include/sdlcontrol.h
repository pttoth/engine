/** -----------------------------------------------------------------------------
  * FILE:    sdlcontrol.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Provides a (not-yet) thread-safe wrapper for SDL functionality
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <mutex>

#include "SDL2/SDL.h"

struct SDL_Window;
struct SDL_Renderer;

namespace engine{

class SDLControl{

public:
    SDLControl();
    SDLControl(const SDLControl& other) = delete;
    SDLControl(SDLControl&& other) = delete;
    virtual ~SDLControl();
    SDLControl& operator=(const SDLControl &other) = delete;
    SDLControl& operator=(SDLControl &&other) = delete;
    bool operator==(const SDLControl &other)const = delete;

    int             WaitEvent(SDL_Event* ev);
    int             PollEvent(SDL_Event* ev);

    int             Init(Uint32 flags);

    SDL_Window*     CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
    SDL_Renderer*   CreateRenderer(SDL_Window *window, int index, Uint32 flags);
    void            DestroyWindow(SDL_Window *window);
    void            DestroyRenderer(SDL_Renderer *renderer);

    SDL_Window*     GetMainWindow();
    SDL_Renderer*   GetMainRenderer();
    void            SetMainWindow(SDL_Window* window);
    void            SetMainRenderer(SDL_Renderer* renderer);

    int             SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int             RenderClear(SDL_Renderer* renderer);
    void            RenderPresent(SDL_Renderer* renderer);
private:

    SDL_Window*     mMainWindow;
    SDL_Renderer*   mMainRenderer;
};

}
