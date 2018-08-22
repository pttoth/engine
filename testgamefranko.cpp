#include "testgamefranko.h"

#include "SDL2/SDL.h"

TestGameFranko::TestGameFranko()
{
    rekt.x = 30;
    rekt.y = 30;
    rekt.w = 30;
    rekt.h = 30;
}

void TestGameFranko::onStart()
{
    sdl_keytester::onStart();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = SDL_CreateWindow("Franko sexy test", 200, 200, 800, 600, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

void TestGameFranko::onExit()
{

}

void TestGameFranko::tick(float t, float dt)
{
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    bool first = true;
    if(first){

        first = false;
    }

    //draw rect
    //...
    SDL_SetRenderDrawColor(renderer, 255,0,0,255);
    SDL_RenderFillRect(renderer, &rekt);

    SDL_RenderPresent(renderer);
}

void TestGameFranko::onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    sdl_keytester::onKeyDown(keycode,keymod,timestamp, repeat);
    switch(keycode){
    case SDLK_w:
        rekt.y += 5;
        break;
    case SDLK_s:
        rekt.y -= 5;
        break;
    case SDLK_a:
        rekt.x -= 5;
        break;
    case SDLK_d:
        rekt.x += 5;
        break;
    default:
        break;
    };
}

void TestGameFranko::onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    sdl_keytester::onKeyDown(keycode,keymod,timestamp, repeat);
}
