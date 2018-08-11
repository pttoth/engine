#include "sdl_keytester.h"

#include <string>
#include <iostream>

std::string getMBname(uint8_t button){
    switch (button){
    case SDL_BUTTON_LEFT:
        return "MBleft";
    case SDL_BUTTON_RIGHT:
        return "MBright";
    case SDL_BUTTON_MIDDLE:
        return "MBmiddle";
    case SDL_BUTTON_X1:
        return "MB_X1";
    case SDL_BUTTON_X2:
        return "MB_X2";
    }
}

std::string dirTostring(uint32_t id){
    switch (id){
    case SDL_MOUSEWHEEL_FLIPPED:
        return "flipped";
        break;
    case SDL_MOUSEWHEEL_NORMAL:
        return "normal";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

std::string mouseidToString(uint32_t id){
    switch (id){
    case SDL_TOUCH_MOUSEID:
        return "TOUCHPAD";
        break;
    default:
        return std::to_string( (unsigned) id);
        break;
    }
}

std::string keycodeToString(SDL_Keycode code){
    return "n/a";
}

std::string keymodToString(uint16_t mod){
    return "n/a";
}

sdl_keytester::sdl_keytester()
{

}

void sdl_keytester::
        onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBdown ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
    std::string bname = getMBname(button);
    std::string mid = mouseidToString(mouseid);
    std::cout << "MBup   ("<<x<<","<<y<<"), " << bname << ", clicks: " <<  std::to_string(clicks) << ", t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid){
    std::string mid = mouseidToString(mouseid);
    //std::cout << "mousemotion(" << x << "," << y << ") rel: (" << x_rel << "," << y_rel << "), t:" << timestamp << ", id:" << mid << std::endl;
}

void sdl_keytester::
        onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction){
    std::string dir = dirTostring(direction);
    std::string mid = mouseidToString(mouseid);
    std::cout << "mousewheel ("<<x<<","<<y<<"), t:" << timestamp << ", id:" << mid << ", dir: " << dir << std::endl;
}

void sdl_keytester::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyDown (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}

void sdl_keytester::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
    std::string key = keycodeToString(keycode);
    std::string mod = keymodToString(keymod);
    std::cout << "KeyUp   (" << key << ", " << mod << " ), t:" << timestamp << ", repeat: " << std::to_string(repeat) << std::endl;
}

void sdl_keytester::
        onTouchInputEvent(){
    std::cout << "Touchpad" << std::endl;
}

void sdl_keytester::onStart()
{
    pttoth::engine::Game::onStart();
    window = SDL_CreateWindow("keytester", 300, 300, 800, 600, NULL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void sdl_keytester::onExit()
{
    pttoth::engine::Game::onExit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void sdl_keytester::
        tick(float t, float dt){
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );
}



