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

    int             WaitEvent(SDL_Event* ev) override;
    int             PollEvent(SDL_Event* ev) override;

    int             Init(Uint32 flags) override;

    SDL_Window*     CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags) override;
    SDL_Renderer*   CreateRenderer(SDL_Window *window, int index, Uint32 flags) override;
    void            DestroyWindow(SDL_Window *window) override;
    void            DestroyRenderer(SDL_Renderer *renderer) override;

    SDL_Window*     GetMainWindow() override;
    SDL_Renderer*   GetMainRenderer() override;
    void            SetMainWindow(SDL_Window* window) override;
    void            SetMainRenderer(SDL_Renderer* renderer) override;

    uint32_t        GetMainWindowWidth() const override;
    uint32_t        GetMainWindowHeight() const override;
    void            SetMainWindowWidth(uint32_t width) override;
    void            SetMainWindowHeight(uint32_t height) override;

    void            GL_SwapWindow( SDL_Window* window ) override;

    //note: Functions taking normalized values don't clamp them into the [0, 1] interval
    //        Non-normalized values will be 1-value off of 'desired' value for every loop around [0, 1]
    //        (eg 1.5f will be 126 instead of 127 | 2.5f will be 125 instead of 127, etc.)
    int             SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a) override;
    int             SetRenderDrawColorNormalized(SDL_Renderer *renderer, float r, float g, float b, float a) override;
    int             SetRenderDrawColorNormalizedF3(SDL_Renderer *renderer, const pt::math::float3& color, float a) override;
    int             SetRenderDrawColorNormalizedF4(SDL_Renderer *renderer, const pt::math::float4& color) override;

    int             SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode) override;

    int             RenderClear(SDL_Renderer* renderer) override;
    void            RenderPresent(SDL_Renderer* renderer) override;

    int             RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect) override;
    int             RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect) override;

    Uint64          SDL_GetTicks64() override;



protected:

private:

    uint32_t        mMainWindowWidth;
    uint32_t        mMainWindowHeight;

    SDL_Window*     mMainWindow;
    SDL_Renderer*   mMainRenderer;
};

}
