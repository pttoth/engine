#include "sdlcontrol.h"

#include "SDL2/SDL.h"

using namespace engine;

SDLControl::
SDLControl():
    mMainWindow(nullptr),
    mMainRenderer(nullptr),
    mMainWindowWidth(0),
    mMainWindowHeight(0)
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


int SDLControl::
SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
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
    SDL_RenderDrawRect(renderer, rect);
}

