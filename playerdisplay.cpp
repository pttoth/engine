#include "playerdisplay.h"

using namespace pttoth;
using namespace pttoth::engine;

PlayerDisplay::
        PlayerDisplay(){
}

PlayerDisplay::
        PlayerDisplay(const PlayerDisplay &other){
}

PlayerDisplay::
        PlayerDisplay(PlayerDisplay &&other){
}

PlayerDisplay::
        ~PlayerDisplay(){
}

PlayerDisplay &PlayerDisplay::
        operator=(const PlayerDisplay &other){
}

PlayerDisplay &PlayerDisplay::
        operator=(PlayerDisplay &&other){
}

bool PlayerDisplay::
        operator==(const PlayerDisplay &other) const{
}

void PlayerDisplay::
        tick(float t, float dt){

}

void PlayerDisplay::
        OnRegistered(){

}

void PlayerDisplay::
        OnUnregistered(){

}

void PlayerDisplay::
        onSpawn(){

}

void PlayerDisplay::
        OnCreateRenderContext(){
    //upload data to GPU memory
}

void PlayerDisplay::
        OnDestroyRenderContext(){
    //remove data from GPU memory
}

