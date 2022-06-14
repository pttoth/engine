#pragma once

#include "engine/Engine.h"

#include "engine/test/ticktester/PrinterComponent.h"
#include "engine/test/ticktester/PrinterEntity.h"
#include "engine/test/ticktester/IListener.h"

#include "engine/SDLControl.h"
#include "engine/Camera2D.h"

#include "PlayerPawn.h"


namespace test{
namespace ticktester{

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

    engine::SDLControl  mSdlControl;
    SDL_Window*         mWindow = nullptr;
    SDL_Renderer*       mRenderer = nullptr;

    engine::Camera2D    mCamera;
    PlayerPawn          mPlayerPawn;

    std::vector<IListener*> mListeners;

    float mDeltaRadius = 0.0f;

    bool mButtonPressedMoveMainDown = false;
    bool mButtonPressedMoveMainUp = false;
    bool mButtonPressedMoveMainLeft = false;
    bool mButtonPressedMoveMainRight = false;

    bool mButtonPressedMoveSubDown = false;
    bool mButtonPressedMoveSubUp = false;
    bool mButtonPressedMoveSubLeft = false;
    bool mButtonPressedMoveSubRight = false;

    bool mButtonPressedIncreaseRadius = false;
    bool mButtonPressedDecreaseRadius = false;

};

} }
