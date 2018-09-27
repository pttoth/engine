#include "progkonfshowcasegame.h"

using namespace pttoth;
using namespace pttoth::engine;

ProgKonfShowcaseGame::
        ProgKonfShowcaseGame(){
    _name = "ProgKonf showcase game";
}

ProgKonfShowcaseGame::
        ~ProgKonfShowcaseGame(){

}

void ProgKonfShowcaseGame::
        onStart(){
    pttoth::engine::Game::onStart();
    window = SDL_CreateWindow("ProgKonf showcase game", 300, 300, 800, 600, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //...
}

void ProgKonfShowcaseGame::
        onExit(){
    //...
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    pttoth::engine::Game::onExit();
}

void ProgKonfShowcaseGame::
        onShutdownSignal(){
}

void ProgKonfShowcaseGame::
        tick(float t, float dt){
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}

void ProgKonfShowcaseGame::
        onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void ProgKonfShowcaseGame::
        onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void ProgKonfShowcaseGame::
        onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid){
}

void ProgKonfShowcaseGame::
        onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction){
}

void ProgKonfShowcaseGame::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){

}

void ProgKonfShowcaseGame::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){

}
