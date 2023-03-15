#pragma once

#include "engine/Engine.h"

#include "engine/SDLManager.h"

namespace test{
namespace ogltester01{

class Game: public engine::Engine
{

public:
    Game();
    virtual ~Game();

protected:
    virtual void OnStart() override;
    virtual void OnExit() override;
    virtual void OnShutdownSignal() override;

    virtual void Update(float t, float dt) override;
    virtual void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    virtual void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    virtual void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

private:
    void InitContext();
    void DestroyContext();

    void InitSdlService();


    bool mInitialized = false;

    engine::SDLManager  mSdlControl;
    SDL_Window*         mWindow = nullptr;
    SDL_GLContext       mGlContext = nullptr;


};

} }
