#include "playercontroller.h"

#include "SDL2/SDL.h"

#include "pttoth/logger.hpp"

using namespace pttoth;

PlayerController::
        PlayerController(){
    _avatar = nullptr;
}

PlayerController::
        PlayerController(const PlayerController &other){
}

PlayerController::
        PlayerController(PlayerController &&other){
}

PlayerController::
        ~PlayerController(){
}

PlayerController &PlayerController::
        operator=(const PlayerController &other){
}

PlayerController &PlayerController::
        operator=(PlayerController &&other){
}

bool PlayerController::
        operator==(const PlayerController &other) const{
}

void PlayerController::
        setAvatar(PlayerAvatar *avatar){

}

void PlayerController::
        onKeyDown(SDL_Keycode keycode,
                  uint16_t keymod,
                  uint32_t timestamp,
                  uint8_t repeat){
    if(0 == repeat){
        if(keycode == _keyMoveForward){     _keydownMoveForward     = true; }
        if(keycode == _keyMoveBackward){    _keydownMoveBackward    = true; }
        if(keycode == _keyMoveLeft){        _keydownMoveLeft        = true; }
        if(keycode == _keyMoveRight){       _keydownMoveRight       = true; }
        if(keycode == _keyFireMain){        _keydownFireMain        = true; }
        if(keycode == _keyFireSpecial){     _keydownFireSpecial     = true; }
        /*
        switch (keycode) {
            case _keyMoveForward:   _keydownMoveForward  = true; break;
            case _keyMoveBackward:  _keydownMoveBackward = true; break;
            case _keyMoveLeft:      _keydownMoveLeft     = true; break;
            case _keyMoveRight:     _keydownMoveRight    = true; break;
            case _keyFireMain:      _keydownFireMain     = true; break;
            case _keyFireSpecial:   _keydownFireSpecial  = true; break;
            default:                break;
        }
        */
    }
}

void PlayerController::
        onKeyUp(SDL_Keycode keycode,
                uint16_t keymod,
                uint32_t timestamp,
                uint8_t repeat){
    if(0 == repeat){
        if(keycode == _keyMoveForward){     _keydownMoveForward     = false; }
        if(keycode == _keyMoveBackward){    _keydownMoveBackward    = false; }
        if(keycode == _keyMoveLeft){        _keydownMoveLeft        = false; }
        if(keycode == _keyMoveRight){       _keydownMoveRight       = false; }
        if(keycode == _keyFireMain){        _keydownFireMain        = false; }
        if(keycode == _keyFireSpecial){     _keydownFireSpecial     = false; }
        /*
        switch (keycode) {
            case _keyMoveForward:   _keydownMoveForward  = false; break;
            case _keyMoveBackward:  _keydownMoveBackward = false; break;
            case _keyMoveLeft:      _keydownMoveLeft     = false; break;
            case _keyMoveRight:     _keydownMoveRight    = false; break;
            case _keyFireMain:      _keydownFireMain     = false; break;
            case _keyFireSpecial:   _keydownFireSpecial  = false; break;
            default:                break;
        }
        */
    }
}

void PlayerController::
        setKey(PlayerController::PlayerAction action, char key){
    switch (action) {
        case PlayerAction::MOVE_FORWARD:    _keyMoveForward = key; break;
        case PlayerAction::MOVE_BACKWARD:   _keyMoveBackward = key; break;
        case PlayerAction::MOVE_LEFT:       _keyMoveLeft = key; break;
        case PlayerAction::MOVE_RIGHT:      _keyMoveRight = key; break;
        case PlayerAction::FIRE_MAIN:       _keyFireMain = key; break;
        case PlayerAction::FIRE_SPECIAL:    _keyFireSpecial = key; break;
        default:                            break;
    }
}

char PlayerController::
        getKey(PlayerController::PlayerAction action) const{
    switch (action) {
        case PlayerAction::MOVE_FORWARD:    return _keyMoveForward;
        case PlayerAction::MOVE_BACKWARD:   return _keyMoveBackward;
        case PlayerAction::MOVE_LEFT:       return _keyMoveLeft;
        case PlayerAction::MOVE_RIGHT:      return _keyMoveRight;
        case PlayerAction::FIRE_MAIN:       return _keyFireMain;
        case PlayerAction::FIRE_SPECIAL:    return _keyFireSpecial;
    }
}

void PlayerController::
        tick(float t, float dt){
    if(_keydownMoveForward){    evMoveForward(t, dt); logger::log << "PlayerController::evMoveForward()\n"; }
    if(_keydownMoveBackward){   evMoveBackward(t, dt); logger::log << "PlayerController::evMoveBackward()\n";}
    if(_keydownMoveLeft){       evMoveLeft(t, dt); logger::log << "PlayerController::evMoveLeft()\n";}
    if(_keydownMoveRight){      evMoveRight(t, dt); logger::log << "PlayerController::evMoveRight()\n";}
    if(_keydownFireMain){       evFireMain(t, dt); logger::log << "PlayerController::evFireMain()\n";}
    if(_keydownFireSpecial){    evFireSpecial(t, dt); logger::log << "PlayerController::evFireSpecial()\n";}
    //logger::log << "PlayerController::tick()\n";
}

void PlayerController::OnRegister()
{

}

void PlayerController::OnUnregister()
{

}

