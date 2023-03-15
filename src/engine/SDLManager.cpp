#include "engine/SDLManager.h"

#include "SDL2/SDL.h"

using namespace engine;

SDLManager::
SDLManager():
    mMainWindowWidth(0),
    mMainWindowHeight(0),
    mMainWindow(nullptr),
    mMainRenderer(nullptr)
{}


SDLManager::
~SDLManager()
{}


int SDLManager::
WaitEvent(SDL_Event *ev)
{
    return SDL_WaitEvent(ev);
}


int SDLManager::
PollEvent(SDL_Event *ev)
{
    return SDL_PollEvent(ev);
}


int SDLManager::
Init(Uint32 flags)
{
    return SDL_Init(flags);
}


SDL_Window *SDLManager::
CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    return SDL_CreateWindow(title, x, y, w, h, flags);
}


SDL_Renderer *SDLManager::
CreateRenderer(SDL_Window *window, int index, Uint32 flags)
{
    return SDL_CreateRenderer(window, index, flags);
}


void SDLManager::
DestroyWindow(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}


void SDLManager::
DestroyRenderer(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}


SDL_Window *SDLManager::
GetMainWindow()
{
    return mMainWindow;
}


SDL_Renderer *SDLManager::
GetMainRenderer()
{
    return mMainRenderer;
}


void SDLManager::
SetMainWindow(SDL_Window *window)
{
    mMainWindow = window;
}


void SDLManager::
SetMainRenderer(SDL_Renderer *renderer)
{
    mMainRenderer = renderer;
}


uint32_t SDLManager::
GetMainWindowWidth() const
{
    return mMainWindowWidth;
}


uint32_t SDLManager::
GetMainWindowHeight() const
{
    return mMainWindowHeight;
}


void SDLManager::
SetMainWindowWidth(uint32_t width)
{
    mMainWindowWidth = width;
}


void SDLManager::
SetMainWindowHeight(uint32_t height)
{
    mMainWindowHeight = height;
}


void SDLManager::
GL_SwapWindow( SDL_Window *window )
{
    SDL_GL_SwapWindow( window );
}


int SDLManager::
SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}


int SDLManager::
SetRenderDrawColorNormalized(SDL_Renderer *renderer, float r, float g, float b, float a)
{
    return this->SetRenderDrawColor(renderer, r*255, g*255, b*255, a*255);
}


int SDLManager::
SetRenderDrawColorNormalizedF3(SDL_Renderer *renderer, const pt::math::float3& color, float a)
{
    return this->SetRenderDrawColorNormalized(renderer, color.x, color.y, color.z, a);
}


int SDLManager::
SetRenderDrawColorNormalizedF4(SDL_Renderer *renderer, const pt::math::float4& color)
{
    return this->SetRenderDrawColorNormalized(renderer, color.x, color.y, color.z, color.w);
}


int SDLManager::
SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode)
{
    return SDL_SetRenderDrawBlendMode(renderer, blendMode);
}


int SDLManager::
RenderClear(SDL_Renderer *renderer)
{
    return SDL_RenderClear(renderer);
}


void SDLManager::
RenderPresent(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
}


int SDLManager::
RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect)
{
    return SDL_RenderDrawRect(renderer, rect);
}


int SDLManager::
RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect)
{
    return SDL_RenderFillRect(renderer, rect);
}


Uint64 SDLManager::
SDL_GetTicks64()
{
    return SDL_GetTicks64();
}

