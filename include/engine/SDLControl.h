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

    virtual int             WaitEvent(SDL_Event* ev) = 0;
    virtual int             PollEvent(SDL_Event* ev) = 0;

    virtual int             Init(Uint32 flags) = 0;

    virtual SDL_Window*     CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags) = 0;
    virtual SDL_Renderer*   CreateRenderer(SDL_Window *window, int index, Uint32 flags) = 0;
    virtual void            DestroyWindow(SDL_Window *window) = 0;
    virtual void            DestroyRenderer(SDL_Renderer *renderer) = 0;

    virtual SDL_Window*     GetMainWindow() = 0;
    virtual SDL_Renderer*   GetMainRenderer() = 0;
    virtual void            SetMainWindow(SDL_Window* window) = 0;
    virtual void            SetMainRenderer(SDL_Renderer* renderer) = 0;

    virtual uint32_t        GetMainWindowWidth() const = 0;
    virtual uint32_t        GetMainWindowHeight() const = 0;
    virtual void            SetMainWindowWidth(uint32_t width) = 0;
    virtual void            SetMainWindowHeight(uint32_t height) = 0;

    virtual void            GL_SwapWindow( SDL_Window* window ) = 0;

    //note: Functions taking normalized values don't clamp them into the [0, 1] interval
    //        Non-normalized values will be 1-value off of 'desired' value for every loop around [0, 1]
    //        (eg 1.5f will be 126 instead of 127 | 2.5f will be 125 instead of 127, etc.)
    virtual int             SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) = 0;
    virtual int             SetRenderDrawColorNormalized(SDL_Renderer *renderer, float r, float g, float b, float a) = 0;
    virtual int             SetRenderDrawColorNormalizedF3(SDL_Renderer *renderer, const pt::math::float3& color, float a) = 0;
    virtual int             SetRenderDrawColorNormalizedF4(SDL_Renderer *renderer, const pt::math::float4& color) = 0;

    virtual int             SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode) = 0;

    virtual int             RenderClear(SDL_Renderer* renderer) = 0;
    virtual void            RenderPresent(SDL_Renderer* renderer) = 0;

    virtual int             RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect) = 0;
    virtual int             RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect) = 0;

    virtual Uint64          SDL_GetTicks64() = 0;



protected:

private:

    uint32_t        mMainWindowWidth;
    uint32_t        mMainWindowHeight;

    SDL_Window*     mMainWindow;
    SDL_Renderer*   mMainRenderer;
};

}
