#include "playeravatar.h"

using namespace pttoth;

PlayerAvatar::
        PlayerAvatar(){
    _ship.setParent( this->getRootComponent() );

}

PlayerAvatar::
        PlayerAvatar(const PlayerAvatar &other){
}

PlayerAvatar::
        PlayerAvatar(PlayerAvatar &&other){
}

PlayerAvatar::
        ~PlayerAvatar(){
}

PlayerAvatar &PlayerAvatar::
        operator=(const PlayerAvatar &other){
}

PlayerAvatar &PlayerAvatar::
        operator=(PlayerAvatar &&other){
}

bool PlayerAvatar::
operator==(const PlayerAvatar &other) const{
}

void PlayerAvatar::tick(float t, float dt)
{

}

void PlayerAvatar::OnRegister()
{

}

void PlayerAvatar::OnUnregister()
{

}
