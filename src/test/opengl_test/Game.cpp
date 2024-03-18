#include "test/opengl_test/Game.h"

#include "engine/Services.h"
#include "engine/SystemManager.h"


#include <thread>

using namespace engine;
using namespace math;

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
/*
    SDL_SetWindowPosition( Services::GetEngineControl()->GetMainWindow(),
                       2000, 32 );
    SDL_SetWindowSize( Services::GetEngineControl()->GetMainWindow(),
                       1600, 900 );
*/
    mBillboardTexture = NewPtr<engine::gl::Texture2d>("mBillboardTexture");
    mBillboardTexture->ReadFilePNG( "../../media/Blade512.png" );
    mBillboardTexture->LoadToVRAM( gl::BufferTarget::TEXTURE_BUFFER, gl::BufferHint::STATIC_DRAW );

    mBillboardActor.CreateRenderContext();
    mBillboardActor.SetPosition( math::float3( 0, 3, 0 ) );
    mBillboardActor.SetOrientation( math::FRotator( -M_PI /2, 0.0f, 0.0f ) );   //TODO: fix, this doesn't work!
    mBillboardActor.SetOrientation( math::FRotator(  0.0f, -M_PI /2, 0.0f ) );  //TODO: fix, this doesn't work!
    mBillboardActor.SetTexture( mBillboardTexture );
    mBillboardActor.Spawn();
    Actor::RegisterTickFunction( mBillboardActor );

    std::thread spin_texture( []{
        while(true){
            break;
        }
    } );
    spin_texture.detach();

/*
    std::thread blink_background( []{
        //WARNING: not threadsafe!
        while(true){
            pt::Sleep( 1000 );
            PT_LOG_OUT( "Switching backgrounds." );
            auto dc = Services::GetDrawingControl();
            auto c = dc->GetClearColor();
            auto newc = c;
            if( c.x == 0.0f ){
                newc = math::float4( 0.2f, 0.2f, 0.2f, 1.0f );
            }else{
                newc = math::float4( math::float3(), 1.0f );
            }
            dc->SetClearColor( newc );
        }

    } );
    blink_background.detach();
*/
    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();

    camera->SetPosition( vec3( 5.0f, 5.0f, 2.0f ) );
    camera->LookAt( vec3::zero ); // look at origo
}


void Game::
OnExit()
{

    Engine::OnExit();
}


void Game::
UpdateGameState( float t, float dt )
{
    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();
    float cameraSpeed = 0.35f;
    math::vec3 movedir;
    bool moved = false;
    if (mForwardDown){
        movedir += camera->GetDir( engine::Camera::Dir::FORWARD );
        moved = true;
    }
    if (mBackDown){
        movedir += camera->GetDir( engine::Camera::Dir::BACKWARD );
        moved = true;
    }
    if (mLeftDown){
        movedir += camera->GetDir( engine::Camera::Dir::LEFT );
        moved = true;
    }
    if (mRightDown){
        movedir += camera->GetDir( engine::Camera::Dir::RIGHT );
        moved = true;
    }
    if (mAscendDown){
        movedir += camera->GetDir( engine::Camera::Dir::UP );
        moved = true;
    }
    if (mDescendDown){
        movedir += camera->GetDir( engine::Camera::Dir::DOWN );
        moved = true;
    }
    if( moved ){
        camera->Move( movedir * cameraSpeed );
    }



}


void Game::
OnMouseButtonDown(int32_t x, int32_t y,
                  uint8_t button, uint8_t clicks,
                  uint32_t timestamp, uint32_t mouseid)
{
    //mMBDown = true;
    mFreeLook = true;
    SDL_SetRelativeMouseMode(SDL_TRUE);
}


void Game::
OnMouseButtonUp(int32_t x, int32_t y,
                uint8_t button, uint8_t clicks,
                uint32_t timestamp, uint32_t mouseid)
{
    //mMBDown = false;
    mFreeLook = false;
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

    if( mFreeLook ){
        //180 pixel = 30 degree = pi/6
        camera->RotateCamera( y_rel * mousespeed_y /180 * static_cast<float>(M_PI) / 6,
                              x_rel * mousespeed_x /180 * static_cast<float>(M_PI) / 6 );
    }

//    assert( false );
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
    switch( keycode ){
    case SDLK_ESCAPE:
        EndMainLoop();
    break;
    case SDLK_w:
        mForwardDown = true;
    break;
    case SDLK_s:
        mBackDown = true;
    break;
    case SDLK_a:
        mLeftDown = true;
    break;
    case SDLK_d:
        mRightDown = true;
    break;
    case SDLK_SPACE:
        mAscendDown = true;
    break;
    case SDLK_LCTRL:
        mDescendDown = true;
    break;
    case SDLK_RCTRL:
        mDescendDown = true;
    break;

    default:
        break;
    }
}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod,
        uint32_t timestamp, uint8_t repeat)
{
    switch( keycode ){
    case SDLK_w:
        mForwardDown = false;
    break;
    case SDLK_s:
        mBackDown = false;
    break;
    case SDLK_a:
        mLeftDown = false;
    break;
    case SDLK_d:
        mRightDown = false;
    break;
    case SDLK_SPACE:
        mAscendDown = false;
    break;
    case SDLK_LCTRL:
        mDescendDown = false;
    break;
    case SDLK_RCTRL:
        mDescendDown = false;
    break;
    default:
        break;
    }
}
