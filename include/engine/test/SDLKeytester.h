#pragma once

#include "engine/Engine.h"

#include "engine/SDLManager.h"

class sdl_keytester: public engine::Engine{

public:
    sdl_keytester();

    virtual void Execute() override;

    // Game interface
protected:
    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnTouchInputEvent() override;

    void OnStart() override;
    void OnExit() override;
    void UpdateGameState(float t, float dt) override;

private:
    engine::SDLManager mSDLc;


    // Engine interface
private:
    virtual void Update() override;
};
