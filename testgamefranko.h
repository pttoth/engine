#pragma once

#include "game.h"
#include "sdl_keytester.h"

class TestGameFranko: public sdl_keytester{

    SDL_Rect rekt;

public:
    TestGameFranko();

    // Application interface
protected:
    void onStart();
    void onExit();

    // Game interface
protected:
    void tick(float t, float dt);
    void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
    void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
};

