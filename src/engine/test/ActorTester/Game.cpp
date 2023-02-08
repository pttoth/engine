#include "engine/test/ActorTester/Game.h"

//#include "engine/experimental/Actor.h"

using namespace test;
using namespace test::actortester;
using namespace engine;
using namespace engine::experimental;


Game::
Game():
    engine::Engine(),
    mPawn( "Pawn" )
{}


Game::
~Game()
{}


void Game::
OnStart()
{
    Engine::OnStart();

}


void Game::
OnExit()
{
    Engine::OnExit();

}


void Game::
Update(float t, float dt)
{
    //Engine::Update(t, dt);

}


void Game::
OnMouseButtonDown( int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseButtonDown( x, y, button, clicks, timestamp, mouseid );

}


void Game::
OnMouseButtonUp( int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseButtonUp( x, y, button, clicks, timestamp, mouseid );

}


void Game::
OnMouseMotion( int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseMotion( x, y, x_rel, y_rel, timestamp, mouseid );

}


void Game::
OnMouseWheel( int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction )
{
    Engine::OnMouseWheel( x, y, timestamp, mouseid, direction );

}


void Game::
OnKeyDown( SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat )
{
    Engine::OnKeyDown( keycode, keymod, timestamp, repeat );

}


void Game::
OnKeyUp( SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat )
{
    Engine::OnKeyUp( keycode, keymod, timestamp, repeat );

}
