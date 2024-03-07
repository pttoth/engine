#include "test/opengl_test/Game.h"

#include "engine/Services.h"
#include "engine/SystemManager.h"


#include <thread>

using namespace engine;

Game::
Game( const int argc, char* argv[] ):
    Engine( argc, argv ),
    mPlayerPawn( "PlayerPawn" ),
    mBillboardActor( "Billboard" )
{}


Game::
~Game()
{}


void Game::
OnStart()
{
    Engine::OnStart();

    mBillboardTexture = NewPtr<engine::gl::Texture2d>("mBillboardTexture");
    mBillboardTexture->ReadFilePNG( "../../media/Blade512.png" );
    mBillboardTexture->LoadToVRAM( gl::BufferTarget::TEXTURE_BUFFER, gl::BufferHint::STATIC_DRAW );

    mBillboardActor.CreateRenderContext();
    mBillboardActor.SetPosition( math::float3( 0, -3, 0 ) );
    mBillboardActor.SetTexture( mBillboardTexture );
    mBillboardActor.Spawn();

    std::thread* asd = new std::thread( []{
        while(true){
            pt::Sleep( 1000 );
            PT_LOG_OUT( "Switching backgrounds." );
            auto dc = Services::GetDrawingControl();
            auto c = dc->GetClearColor();
            auto newc = c;
            if( c.x == 0.0f ){
                newc = math::float4( 0.5f, 0.5f, 0.5f, 0.5f );
            }else{
                newc = math::float4( );
            }
            dc->SetClearColor( newc );
        }

    } );
}


void Game::
OnExit()
{

    Engine::OnExit();
}


void Game::
UpdateGameState( float t, float dt )
{}


void Game::
OnMouseButtonDown(int32_t x, int32_t y,
                  uint8_t button, uint8_t clicks,
                  uint32_t timestamp, uint32_t mouseid)
{
    mMBDown = true;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}


void Game::
OnMouseButtonUp(int32_t x, int32_t y,
                uint8_t button, uint8_t clicks,
                uint32_t timestamp, uint32_t mouseid)
{
    mMBDown = false;
    SDL_SetRelativeMouseMode(SDL_FALSE);
}


void Game::
OnMouseMotion(int32_t x, int32_t y,
              int32_t x_rel, int32_t y_rel,
              uint32_t timestamp, uint32_t mouseid)
{
    float mousespeed_x = 0.30f;
    float mousespeed_y = 0.30f;

    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();

    assert( false );
/*
    if( mMBDown ){
        //180 pixel = 30 degree = pi/6
        camera->moveTarget(-x_rel * mousespeed_x /180 * static_cast<float>(M_PI) / 6,
                           -y_rel * mousespeed_y /180 * static_cast<float>(M_PI) / 6);
    }
*/
}


void Game::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod,
          uint32_t timestamp, uint8_t repeat)
{

}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod,
        uint32_t timestamp, uint8_t repeat)
{

}
