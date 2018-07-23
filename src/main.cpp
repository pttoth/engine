/** -----------------------------------------------------------------------------
  * FILE:    main.cpp
  * AUTHOR:  pttoth - 2018.07.23.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#include <random>
#include <time.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_events.h"

int main(int argc, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Event ev;
    SDL_Window* window;
    SDL_Renderer* renderer;

    size_t winx = 32;
    size_t winy = 32;
    size_t winw = 640;
    size_t winh = 480;
    window = SDL_CreateWindow("anyad",
                              winx, winy,
                              winw, winh,
                              SDL_WINDOW_OPENGL
                              |SDL_WINDOW_BORDERLESS);

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED
                                  |SDL_RENDERER_PRESENTVSYNC);

    char red, green, blue;
    red = 255;
    green = 255;
    blue = 255;

    srand(time(NULL));


    int counter = 0;
    bool done = false;
    while(!done){
        SDL_PollEvent(&ev);
        if(ev.type == SDL_MOUSEBUTTONDOWN){
            done = true;
        }
        if(0 == counter % 10){
            red = rand()*255;
            green = rand()*255;
            blue = rand()*255;

            SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
        ++counter;
        SDL_Delay(25);
    }

    SDL_Quit();

    return 0;
}
