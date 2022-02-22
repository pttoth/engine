#include "sdlcontrol.h"

#include "sdl2/SDL.h"

SDLControl::
        SDLControl(){

}

SDLControl::
        ~SDLControl(){

}


int SDLControl::
        WaitEvent(SDL_Event *ev){
    std::lock_guard<std::mutex> lock(_mut_events);
    return SDL_WaitEvent(ev);
}

int SDLControl::
        PollEvent(SDL_Event *ev){
    std::lock_guard<std::mutex> lock(_mut_events);
    return SDL_PollEvent(ev);
}

int SDLControl::Init(Uint32 flags){
    std::lock_guard<std::mutex> lock_ev(_mut_events);
    std::lock_guard<std::mutex> lock_graph(_mut_graphics);
    return SDL_Init(flags);
}

SDL_Window *SDLControl::CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags){
    std::lock_guard<std::mutex> lock(_mut_graphics);
    return SDL_CreateWindow(title, x, y, w, h, flags);
}

SDL_Renderer *SDLControl::CreateRenderer(SDL_Window *window, int index, Uint32 flags){
    std::lock_guard<std::mutex> lock(_mut_graphics);
    return SDL_CreateRenderer(window, index, flags);
}

int SDLControl::SetRenderDrawColor(SDL_Renderer *renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
    std::lock_guard<std::mutex> lock(_mut_graphics);
    return SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

int SDLControl::RenderClear(SDL_Renderer *renderer){
    std::lock_guard<std::mutex> lock(_mut_graphics);
    return SDL_RenderClear(renderer);
}

void SDLControl::RenderPresent(SDL_Renderer *renderer){
    std::lock_guard<std::mutex> lock(_mut_graphics);
    SDL_RenderPresent(renderer);
}

