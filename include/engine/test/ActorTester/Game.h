#pragma once

#include "engine/test/ActorTester/PlayerPawn.h"

#include "engine/Engine.h"

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

protected:
    void OnStart() override;
    void OnExit() override;

    void Update(float t, float dt) override;
    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

private:
    engine::experimental::PlayerPawn mPawn;


};

} //end of namespace experimental
} //end of namespace engine