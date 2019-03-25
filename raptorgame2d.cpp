#include "raptorgame2d.h"

using namespace pttoth;

RaptorGame2D::
        RaptorGame2D(){
    _window = nullptr;
    _renderer = nullptr;
    _config_filename = "../../RaptorGame.cfg";
    cfgAddKey(_config_raptorgame, CfgKey::cKeyMoveForward);
    cfgAddKey(_config_raptorgame, CfgKey::cKeyMoveBackward);
    cfgAddKey(_config_raptorgame, CfgKey::cKeyMoveLeft);
    cfgAddKey(_config_raptorgame, CfgKey::cKeyMoveRight);
    cfgAddKey(_config_raptorgame, CfgKey::cKeyFire);
    cfgAddKey(_config_raptorgame, CfgKey::cKeySpecialFire);
    cfgAddKey(_config_raptorgame, CfgKey::fPlayerSpeed);

}

RaptorGame2D::
        RaptorGame2D(const RaptorGame2D &other){
}

RaptorGame2D::
        RaptorGame2D(RaptorGame2D &&other){
}

RaptorGame2D::
        ~RaptorGame2D(){
}

RaptorGame2D &RaptorGame2D::
        operator=(const RaptorGame2D &other){
}

RaptorGame2D &RaptorGame2D::
        operator=(RaptorGame2D &&other){
}

bool RaptorGame2D::
operator==(const RaptorGame2D &other) const{
}

void RaptorGame2D::
        onStart(){
    Game::onStart();
    _window = SDL_CreateWindow("Raptor game",
                                32, 32,
                               1360, 768, NULL
                               |SDL_WINDOW_OPENGL
                               //|SDL_WINDOW_FULLSCREEN
                               );
    _renderer = SDL_CreateRenderer(_window,
                                   -1,
                                   SDL_RENDERER_ACCELERATED);

    math::float2 startingpos(780, 540);
    _playerpos = startingpos;

    engine::Entity::RegisterEntity(&_projsys);
    engine::Entity::RegisterTickFunction(&_projsys,
                                         engine::TickGroup::DURINGPHYSICS);

    _config_raptorgame.setPath(_config_filename);
    _config_raptorgame.read();
    //set up variables from config
    //...

}

void RaptorGame2D::
        onExit(){

    engine::Entity::UnregisterTickFunction(&_projsys);
    engine::Entity::UnregisterEntity(&_projsys);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    Game::onExit();
}

void RaptorGame2D::
        onShutdownSignal(){
    Game::onShutdownSignal();
}

void RaptorGame2D::
        tick(float t, float dt){


    drawScene(t, dt);
}

void RaptorGame2D::onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}

void RaptorGame2D::onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}

void RaptorGame2D::
        drawScene(float t, float dt){
    //clear screen
    if(_renderer){ SDL_RenderClear(_renderer); }

    //draw contents
    //...

    //update frame
    if(_renderer){ SDL_RenderPresent(_renderer); }
}
