#ifndef SDL_KEYTESTER_H
#define SDL_KEYTESTER_H

#include "engine.h"

class sdl_keytester: public engine::Engine{

public:
    sdl_keytester();


    // Game interface
protected:
    void onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid);
    void onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid);
    void onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid);
    void onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction);
    void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
    void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
    void onTouchInputEvent();


protected:
    void onStart();
    void onExit();
    void updateGameState(float t, float dt);

    // Game interface
protected:
    void tick(float t, float dt)override;
};

#endif // SDL_KEYTESTER_H
