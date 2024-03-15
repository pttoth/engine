#pragma once

#include "engine/Engine.h"
#include "engine/SystemManager.h"
#include "engine/gl/Texture.h"
#include "pt/macros.h"
#include "BillboardActor.h"
#include "Pawn.h"

PT_FORWARD_DECLARE_CLASS( Game )

class Game: public engine::Engine
{
public:
    Game() = delete;
    Game( int const argc, char* argv[] );
    Game( const Game& other ) = delete;
    Game( Game&& source ) = delete;
    virtual ~Game();
    Game& operator=( const Game& other ) = delete;
    Game& operator=( Game&& source ) = delete;
    bool operator==( const Game& other ) const = delete;
protected:
    void OnStart() override;
    void OnExit() override;
    void UpdateGameState( float t, float dt ) override;

    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

private:
    bool mFreeLook = false;
    bool mMBDown = false;
    bool mForwardDown = false;
    bool mBackDown = false;
    bool mLeftDown = false;
    bool mRightDown = false;
    bool mAscendDown = false;
    bool mDescendDown = false;

    engine::gl::Texture2dPtr mBillboardTexture;

    Pawn            mPlayerPawn;
    BillboardActor  mBillboardActor;


};
