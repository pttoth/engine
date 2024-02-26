#include "test/opengl_test/Game.h"


Game::
Game( const int argc, char* argv[] ):
    Engine( argc, argv ),
    mPlayerPawn( "PlayerPawn" )
//    mBillboard( "Billboard" )
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
UpdateGameState( float t, float dt )
{}
