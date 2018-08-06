#include "game.h"

#include <assert.h>
#include "events.h"

#include "SDL2/SDL.h"

using namespace pttoth;
using namespace engine;

Uint32 generate_gametimer_tick(Uint32 interval, void *param){
    SDL_Event ev;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = game_event::EV_GAMETIMER_TICK;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    ev.user = userevent;

    SDL_PushEvent(&ev);
    return(interval);
}



Game::
        Game(): Application(),
                 window(nullptr), renderer(nullptr),
                _uptime(0){
}

pttoth::engine::Game::
        Game(int const argc, char* argv[]):
            Application(argc, argv),
            window(nullptr), renderer(nullptr),
            _uptime{0}{
    int init = SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO);
    if( 0 != init  ){
        setErrorMessage("Failed to initialize SDL timer");
    }
    _uptime = SDL_GetTicks();
}

pttoth::engine::Game::
        ~Game(){
}

void pttoth::engine::Game::
        onStart(){
    Application::onStart();
    Uint32 interval = (Uint32) (1000.0f / _tickrate);

    // there seems to be a problem here ( thread desnyc? )
    //  without the delay, the timer doesn't always start
    SDL_Delay(150);

    _gametimer_id = SDL_AddTimer(interval, generate_gametimer_tick, nullptr);

    atexit(SDL_Quit);
}

void pttoth::engine::Game::
        onExit(){
    Application::onExit();
    SDL_RemoveTimer(_gametimer_id);
}

void pttoth::engine::Game::
    updateGameState( float t, float dt ){

}

void Game::
        quit(){
    SDL_Event ev;
    BuildUserEvent(&ev,game_event::EV_APP_QUIT, nullptr, nullptr);
    SDL_PushEvent(&ev);
}

void Game::onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{

}

void Game::onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{

}

void Game::
        onMouseMotion(int32_t x, int32_t y,
                      int32_t x_rel, int32_t y_rel,
                      uint32_t timestamp, uint32_t mouseid){

}

void Game::onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{

}

void Game::onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}

void Game::onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}

void Game::
        onTouchEvent(){
    assert(false);
}


void Game::
        processGametimerTick(){
    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - _uptime) / 1000.0f;
    updateGameState(ft, fdt);
    _uptime = current_time;
}

void pttoth::engine::Game::
        onEvent(SDL_Event* event){
    SDL_Event ev = *event; //avoid unneccessary repeat of dereferences
    switch(ev.type){
    case SDL_MOUSEMOTION:
        if(! (ev.motion.which & SDL_TOUCH_MOUSEID) ){ //if not touchpad event
            onMouseMotion(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel, ev.motion.timestamp, ev.motion.which);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown(ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which);
        break;
    case SDL_MOUSEBUTTONUP:
        onMouseButtonUp(ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which);
        break;
    case SDL_MOUSEWHEEL:
        onMouseWheel(ev.wheel.x, ev.wheel.y, ev.wheel.timestamp, ev.wheel.which, ev.wheel.direction);
        break;
    case SDL_KEYDOWN:
        onKeyDown(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat);
        break;
    case SDL_KEYUP:
        onKeyUp(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat);
        break;
    case SDL_FINGERMOTION:
        //TODO: handle...
        break;
    case SDL_FINGERDOWN:
        //TODO: handle...
        break;
    case SDL_FINGERUP:
        //TODO: handle...
        break;
    case SDL_USEREVENT:
        switch( ev.user.code ){
        case game_event::EV_GAMETIMER_TICK:
            processGametimerTick();
            break;
        default:
            break;
        }
        break;
    case SDL_QUIT:
        quit();
        break;
    }
}
