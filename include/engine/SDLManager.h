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
    SDLManager(const SDLManager& other) = delete;
    SDLManager(SDLManager&& other) = delete;
    virtual ~SDLManager();
    SDLManager& operator=(const SDLManager &other) = delete;
    SDLManager& operator=(SDLManager &&other) = delete;
    bool operator==(const SDLManager &other)const = delete;

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

    uint32_t        GetMainWindowWidth() const;
    uint32_t        GetMainWindowHeight() const;
    void            SetMainWindowWidth(uint32_t width);
    void            SetMainWindowHeight(uint32_t height);

    void            GL_SwapWindow( SDL_Window* window );

    //note: Functions taking normalized values don't clamp them into the [0, 1] interval
    //        Non-normalized values will be 1-value off of 'desired' value for every loop around [0, 1]
    //        (eg 1.5f will be 126 instead of 127 | 2.5f will be 125 instead of 127, etc.)
    int             SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    int             SetRenderDrawColorNormalized(SDL_Renderer *renderer, float r, float g, float b, float a);
    int             SetRenderDrawColorNormalizedF3(SDL_Renderer *renderer, const pt::math::float3& color, float a);
    int             SetRenderDrawColorNormalizedF4(SDL_Renderer *renderer, const pt::math::float4& color);

    int             SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode);

    int             RenderClear(SDL_Renderer* renderer);
    void            RenderPresent(SDL_Renderer* renderer);

    int             RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect);
    int             RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect);

    Uint64          SDL_GetTicks64();



protected:

private:

    uint32_t        mMainWindowWidth;
    uint32_t        mMainWindowHeight;

    SDL_Window*     mMainWindow;
    SDL_Renderer*   mMainRenderer;
};

}
