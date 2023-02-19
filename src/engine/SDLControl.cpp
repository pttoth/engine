#include "engine/SDLControl.h"

#include "SDL2/SDL.h"

using namespace engine;

SDLControl::
SDLControl():
    mMainWindowWidth(0),
    mMainWindowHeight(0),
    mMainWindow(nullptr),
    mMainRenderer(nullptr)
{}


SDLControl::
~SDLControl()
{}


int SDLControl::
WaitEvent(SDL_Event *ev)
{
    return SDL_WaitEvent(ev);
}


int SDLControl::
PollEvent(SDL_Event *ev)
{
    return SDL_PollEvent(ev);
}


int SDLControl::
Init(Uint32 flags)
{
    return SDL_Init(flags);
}


SDL_Window *SDLControl::
CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    return SDL_CreateWindow(title, x, y, w, h, flags);
}


SDL_Renderer *SDLControl::
CreateRenderer(SDL_Window *window, int index, Uint32 flags)
{
    return SDL_CreateRenderer(window, index, flags);
}


void SDLControl::
DestroyWindow(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}


void SDLControl::
DestroyRenderer(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}


SDL_Window *SDLControl::
GetMainWindow()
{
    return mMainWindow;
}


SDL_Renderer *SDLControl::
GetMainRenderer()
{
    return mMainRenderer;
}


void SDLControl::
SetMainWindow(SDL_Window *window)
{
    mMainWindow = window;
}


void SDLControl::
SetMainRenderer(SDL_Renderer *renderer)
{
    mMainRenderer = renderer;
}


uint32_t SDLControl::
GetMainWindowWidth() const
{
    return mMainWindowWidth;
}


uint32_t SDLControl::
GetMainWindowHeight() const
{
    return mMainWindowHeight;
}


void SDLControl::
SetMainWindowWidth(uint32_t width)
{
    mMainWindowWidth = width;
}


void SDLControl::
SetMainWindowHeight(uint32_t height)
{
    mMainWindowHeight = height;
}


void SDLControl::
GL_SwapWindow( SDL_Window *window )
{
    SDL_GL_SwapWindow( window );
}


int SDLControl::
SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}


int SDLControl::
SetRenderDrawColorNormalized(SDL_Renderer *renderer, float r, float g, float b, float a)
{
    return this->SetRenderDrawColor(renderer, r*255, g*255, b*255, a*255);
}


int SDLControl::
SetRenderDrawColorNormalizedF3(SDL_Renderer *renderer, const pt::math::float3& color, float a)
{
    return this->SetRenderDrawColorNormalized(renderer, color.x, color.y, color.z, a);
}


int SDLControl::
SetRenderDrawColorNormalizedF4(SDL_Renderer *renderer, const pt::math::float4& color)
{
    return this->SetRenderDrawColorNormalized(renderer, color.x, color.y, color.z, color.w);
}


int SDLControl::
SetRenderDrawBlendMode(SDL_Renderer *renderer, SDL_BlendMode blendMode)
{
    return SDL_SetRenderDrawBlendMode(renderer, blendMode);
}


int SDLControl::
RenderClear(SDL_Renderer *renderer)
{
    return SDL_RenderClear(renderer);
}


void SDLControl::
RenderPresent(SDL_Renderer *renderer)
{
    SDL_RenderPresent(renderer);
}


int SDLControl::
RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect)
{
    return SDL_RenderDrawRect(renderer, rect);
}


int SDLControl::
RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect)
{
    return SDL_RenderFillRect(renderer, rect);
}

