#include "sdlcontrol.h"

#include "SDL2/SDL.h"

SDLControl::
SDLControl()
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

