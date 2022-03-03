#pragma once

#include "engine.h"

#include "sdlcontrol.h"

#include "test/ticktester/printercomponent.h"
#include "test/ticktester/printerentity.h"


namespace test{
namespace ticktester{

class Game: public engine::Engine
{

public:
    Game();
    virtual ~Game();

protected:
    virtual void onStart() override;
    virtual void onExit() override;
    virtual void onShutdownSignal() override;

    virtual void tick(float t, float dt) override;
    virtual void onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    virtual void onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    virtual void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    virtual void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

private:
    void InitContext();
    void DestroyContext();
    bool mInitialized;

    void InitSdlService();

    engine::SDLControl  mSdlControl;
    SDL_Window*         mWindow;
    SDL_Renderer*       mRenderer;



};

} }
