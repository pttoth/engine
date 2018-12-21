#include "player.h"

using namespace pttoth;
using namespace pttoth::engine;

Player::
        Player(){
    addComponent( &_displaycomponent );
    _displaycomponent.setParent( getRootComponent() );
}

Player::
        Player(const Player &other){
}

Player::
        Player(Player &&other){
}

Player::
        ~Player(){
}

Player &Player::
        operator=(const Player &other){
}

Player &Player::
        operator=(Player &&other){
}

bool Player::
        operator==(const Player &other) const{
}

void Player::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod,
                  uint32_t timestamp, uint8_t repeat){
    switch(keycode){
        case SDLK_w:
            dir.y = move_speed;
            break;
        case SDLK_s:
            dir.y = move_speed * -1;
            break;
        case SDLK_a:
            dir.x = move_speed;
            break;
        case SDLK_d:
            dir.x = move_speed * -1;
            break;
        default:
            break;
    }
}

void Player::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod,
                uint32_t timestamp, uint8_t repeat){
    switch(keycode){
        case SDLK_w:
            dir.y = 0;
            break;
        case SDLK_s:
            dir.y = 0;
            break;
        case SDLK_a:
            dir.x = 0;
            break;
        case SDLK_d:
            dir.x = 0;
            break;
        default:
            break;
    }
}

void Player::
        tick(float t, float dt){
    math::float3 newpos = getRootComponent()->getPosition() + dir;
    getRootComponent()->setPosition( newpos );
}

void Player::
        OnRegister(){
    Entity::RegisterTickFunction(this, TickGroup::PREPHYSICS);
    //...

}

void Player::
        OnUnregister(){
    //...
    Entity::UnregisterTickFunction(this);
}

void Player::
        OnSpawn(){

}
