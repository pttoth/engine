#pragma once

#include "engine/test/ActorTester/PlayerPawn.h"
#include "engine/Engine.h"

#include "engine/SDLManager.h"
#include "engine/SerialScheduler.h"

#include "engine/DrawingManager.h"

#include "engine/CameraOrtographic.h"

namespace test{
namespace actortester{

class Game: public engine::Engine
{
public:
    Game();
    Game( const Game& other ) = delete;
    Game( Game&& source ) = delete;

    virtual ~Game();

    Game& operator=( const Game& other ) = delete;
    Game& operator=( Game&& source ) = delete;

    bool operator==( const Game& other ) const = delete;

    void Execute() override;

protected:
    void OnStart() override;
    void OnExit() override;

    void UpdateGameState(float t, float dt) override;
    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

private:
    void InitContext();
    void DestroyContext();

    void InitSdlService();

    static const char* VertexShader;
    static const char* GeometryShader;
    static const char* FragmentShader;


    bool mInitialized = false;

    engine::CameraOrtographic    mCamera;

    engine::SDLManager  mSdlControl;
    SDL_GLContext       mGlContext = nullptr;

    engine::PlayerPawn        mPawn;


    bool mButtonPressedMoveMainDown = false;
    bool mButtonPressedMoveMainUp = false;
    bool mButtonPressedMoveMainLeft = false;
    bool mButtonPressedMoveMainRight = false;

    bool mButtonPressedMoveSubDown = false;
    bool mButtonPressedMoveSubUp = false;
    bool mButtonPressedMoveSubLeft = false;
    bool mButtonPressedMoveSubRight = false;


};

} //end of namespace actortester
} //end of namespace engine
