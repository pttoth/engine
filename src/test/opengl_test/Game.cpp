#include "test/opengl_test/Game.h"

#include "engine/Services.h"
#include "engine/SystemManager.h"

using namespace engine;

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

    auto syscontrol = Services::GetSystemControl();
    /*
    PT_LOG_OUT( syscontrol->GetLibPNGInfo() );
    PT_LOG_OUT( syscontrol->GetZLibInfo() );
    PT_LOG_OUT( syscontrol->GetGraphicsAPIInfo() );
    */
}


void Game::
OnExit()
{

    Engine::OnExit();
}


void Game::
UpdateGameState( float t, float dt )
{}
