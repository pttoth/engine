#include "services.h"

using namespace engine;

Services* Services::_instance = nullptr;

Services::
        Services(){
}

Services::
        Services(const Services &other){
}

Services* Services::
        instance(){
    if(nullptr == _instance){
        _instance = new Services();
    }
    return _instance;
}

World *Services::
        getWorld(){
    return instance()->_world;
}

void Services::
        setWorld(World *world){
    instance()->_world = world;
}

SDLControl *Services::
        getSDLControl(){
    return instance()->_sdl_control;
}

void Services::
        setSDLControl(SDLControl *sdl_control){
    instance()->_sdl_control = sdl_control;
}

GameControl *Services::
        getGameControl(){
    return instance()->_game_control;
}

void Services::
        setGameControl(GameControl *game_control){
    instance()->_game_control = game_control;
}
