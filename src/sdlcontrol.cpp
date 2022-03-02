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
    std::lock_guard<std::mutex> lock(mMutEvents);
    return SDL_WaitEvent(ev);
}

int SDLControl::
PollEvent(SDL_Event *ev)
{
    std::lock_guard<std::mutex> lock(mMutEvents);
    return SDL_PollEvent(ev);
}

int SDLControl::
Init(Uint32 flags)
{
    std::lock_guard<std::mutex> lock_ev(mMutEvents);
    std::lock_guard<std::mutex> lock_graph(mMutGraphics);
    return SDL_Init(flags);
}

SDL_Window *SDLControl::
CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    std::lock_guard<std::mutex> lock(mMutGraphics);
    return SDL_CreateWindow(title, x, y, w, h, flags);
}

SDL_Renderer *SDLControl::
CreateRenderer(SDL_Window *window, int index, Uint32 flags)
{
    std::lock_guard<std::mutex> lock(mMutGraphics);
    return SDL_CreateRenderer(window, index, flags);
}

int SDLControl::
SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    std::lock_guard<std::mutex> lock(mMutGraphics);
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

int SDLControl::
RenderClear(SDL_Renderer *renderer)
{
    std::lock_guard<std::mutex> lock(mMutGraphics);
    return SDL_RenderClear(renderer);
}

void SDLControl::
RenderPresent(SDL_Renderer *renderer)
{
    std::lock_guard<std::mutex> lock(mMutGraphics);
    SDL_RenderPresent(renderer);
}

