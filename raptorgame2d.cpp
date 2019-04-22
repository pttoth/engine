#include "raptorgame2d.h"

#include "SDL2/SDL.h"
#include "pttoth/logger.hpp"


using namespace pttoth;

RaptorGame2D::
        RaptorGame2D(){
    _window = nullptr;
    _renderer = nullptr;
    _config_filename = "../../cfg/RaptorGame.cfg";
    _config_default_filename = "../../cfg/DefaultRaptorGame.cfg";
    cfgAddKey(_config_raptorgame, cKeyMoveForward);
    cfgAddKey(_config_raptorgame, cKeyMoveBackward);
    cfgAddKey(_config_raptorgame, cKeyMoveLeft);
    cfgAddKey(_config_raptorgame, cKeyMoveRight);
    cfgAddKey(_config_raptorgame, cKeyFire);
    cfgAddKey(_config_raptorgame, cKeySpecialFire);
    cfgAddKey(_config_raptorgame, fPlayerSpeed);

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
                               //1360, 768, NULL
                               640, 480, NULL
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
    try{
        //try standard config file
        _config_raptorgame.read(); //throws std::invalid_argument if file is missing
        _setLocalVariablesFromConfig();
    }catch(std::invalid_argument& e){
        logger::log << "error while reading config file: " << _config_filename
                    << "\n\treason: " << e.what() << "\n";
        logger::log << "\treading default config instead\n";
        try{
            //try default settings from default config file
            _config_raptorgame.readF(_config_default_filename); //throws std::invalid_argument if file is missing
            _setLocalVariablesFromConfig();
        }catch(std::invalid_argument& e){
            logger::log << "error while reading default config file: " << _config_default_filename
                        << "\n\treason: " << e.what() << "\n";
            logger::log << "\tsetting up default values instead\n";
            //set hardcoded default settings
            _setDefaultConfigValues();
        }
    }

    _playercontroller.evMoveForward.add(    this, _playerMoveForward);
    _playercontroller.evMoveBackward.add(   this, _playerMoveBackward);
    _playercontroller.evMoveLeft.add(       this, _playerMoveLeft);
    _playercontroller.evMoveRight.add(      this, _playerMoveRight);
    _playercontroller.evFireMain.add(       this, _playerFireMain);
    _playercontroller.evFireSpecial.add(    this, _playerFireSpecial);

    engine::Entity::RegisterEntity(&_playercontroller);
    engine::Entity::RegisterTickFunction(&_playercontroller,
                                         engine::TickGroup::PREPHYSICS);

}

void RaptorGame2D::
        onExit(){

    _setConfigFromLocalVariables();
    _config_raptorgame.write();
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
    count = (count +1) % 30;

    if(count < 1){
        drawScene(t, dt);
    }

}

void RaptorGame2D::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod,
                  uint32_t timestamp, uint8_t repeat){
    _playercontroller.onKeyDown(keycode, keymod, timestamp, repeat);
}

void RaptorGame2D::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod,
                uint32_t timestamp, uint8_t repeat){
    _playercontroller.onKeyUp(keycode, keymod, timestamp, repeat);
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

void RaptorGame2D::
        _setDefaultConfigValues(){
    //set member variables
    logger::log << "_setDefaultConfigValues called\n";
    _playercontroller.setKey(PlayerController::PlayerAction::MOVE_FORWARD, 'w');
    _playercontroller.setKey(PlayerController::PlayerAction::MOVE_BACKWARD, 's');
    _playercontroller.setKey(PlayerController::PlayerAction::MOVE_LEFT, 'a');
    _playercontroller.setKey(PlayerController::PlayerAction::MOVE_RIGHT, 'd');
    _playercontroller.setKey(PlayerController::PlayerAction::FIRE_MAIN, ' ');
    _playercontroller.setKey(PlayerController::PlayerAction::FIRE_SPECIAL, SDLK_LCTRL);

    _setConfigFromLocalVariables();
}

void RaptorGame2D::
        _setConfigFromLocalVariables(){
    std::string c[6];
    c[0] = _playercontroller.getKey(PlayerController::PlayerAction::MOVE_FORWARD);
    c[1] = _playercontroller.getKey(PlayerController::PlayerAction::MOVE_BACKWARD);
    c[2] = _playercontroller.getKey(PlayerController::PlayerAction::MOVE_LEFT);
    c[3] = _playercontroller.getKey(PlayerController::PlayerAction::MOVE_RIGHT);
    c[4] = _playercontroller.getKey(PlayerController::PlayerAction::FIRE_MAIN);
    c[5] = _playercontroller.getKey(PlayerController::PlayerAction::FIRE_SPECIAL);

    _config_raptorgame.setS(cKeyMoveForward,    c[0]);
    _config_raptorgame.setS(cKeyMoveBackward,   c[1]);
    _config_raptorgame.setS(cKeyMoveLeft,       c[2]);
    _config_raptorgame.setS(cKeyMoveRight,      c[3]);
    _config_raptorgame.setS(cKeyFire,           c[4]);
    _config_raptorgame.setS(cKeySpecialFire,    c[5]);
}

void RaptorGame2D::_setLocalVariablesFromConfig()
{

}

void RaptorGame2D::
        _playerMoveForward(float t, float dt){
    logger::debug << "_playerMoveForward\n";
}

void RaptorGame2D::
        _playerMoveBackward(float t, float dt){
    logger::debug << "_playerMoveBackward\n";
}

void RaptorGame2D::_playerMoveLeft(float t, float dt)
{
    logger::debug << "_playerMoveLeft\n";
}

void RaptorGame2D::_playerMoveRight(float t, float dt)
{
    logger::debug << "_playerMoveRight\n";
}

void RaptorGame2D::_playerFireMain(float t, float dt)
{
    logger::debug << "_playerFireMain\n";
}

void RaptorGame2D::_playerFireSpecial(float t, float dt)
{
    logger::debug << "_playerFireSpecial\n";
}
