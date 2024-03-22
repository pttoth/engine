#include "test/opengl_test/Game.h"

#include "engine/Services.h"
#include "engine/SystemManager.h"


#include <thread>

using namespace engine;
using namespace math;

Game::
Game( const int argc, char* argv[] ):
    Engine( argc, argv ),
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
    mBillboardTexture->LoadToVRAM();

    mBillboardActor.CreateRenderContext();
    mBillboardActor.SetTexture( mBillboardTexture );
    mBillboardActor.Spawn();
    Actor::RegisterTickFunction( mBillboardActor );

    auto dc = Services::GetDrawingControl();
    auto shp = dc->GetDefaultShaderProgram();
    dc->SetWireframeMode( 0 );

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
    float cameraSpeed = 10.0f * dt;
    math::vec3 movedir;
    bool cameramoved = false;
    if (mForwardDown){
        movedir += camera->GetDir( engine::Camera::Dir::FORWARD );
        cameramoved = true;
    }
    if (mBackDown){
        movedir += camera->GetDir( engine::Camera::Dir::BACKWARD );
        cameramoved = true;
    }
    if (mLeftDown){
        movedir += camera->GetDir( engine::Camera::Dir::LEFT );
        cameramoved = true;
    }
    if (mRightDown){
        movedir += camera->GetDir( engine::Camera::Dir::RIGHT );
        cameramoved = true;
    }
    if (mAscendDown){
        movedir += camera->GetDir( engine::Camera::Dir::UP );
        cameramoved = true;
    }
    if (mDescendDown){
        movedir += camera->GetDir( engine::Camera::Dir::DOWN );
        cameramoved = true;
    }
    if( cameramoved ){
        if( 0.0001f < movedir.length()  ){
            camera->Move( movedir.normalize() * cameraSpeed );
        }
    }


    float PawnSpeed = 10.0f * dt;
    math::vec3 pawnMoveDir;
    bool pawnMoved = false;
    if (mUpArrowDown){
        pawnMoveDir += vec3::xUnit * -1;
        pawnMoved = true;
    }
    if (mDownArrowDown){
        pawnMoveDir += vec3::xUnit;
        pawnMoved = true;
    }
    if (mLeftArrowDown){
        pawnMoveDir += vec3::yUnit * -1;
        pawnMoved = true;
    }
    if (mRightArrowDown){
        pawnMoveDir += vec3::yUnit;
        pawnMoved = true;
    }
    if (mHomeDown){
        pawnMoveDir += vec3::zUnit;
        pawnMoved = true;
    }
    if (mEndDown){
        pawnMoveDir += vec3::zUnit * -1;
        pawnMoved = true;
    }
    if( pawnMoved ){
        if( 0.0001f < pawnMoveDir.length()  ){
            vec3 pos = mBillboardActor.GetPosition();
            mBillboardActor.SetPosition( pos + pawnMoveDir.normalize() * PawnSpeed );
        }
    }

    FRotator Xrot( 2.5f *t, 0, 0 );
    mBillboardActor.SetOrientation( Xrot );
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
}


void Game::
OnMouseWheel( int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction )
{
    auto dc = Services::GetDrawingControl();
    auto shp = dc->GetDefaultShaderProgram();
    static int mode = 0;
    if( 0 < y ){
        mode = (mode+1) %3;
    }else{
        mode = (mode-1+3) %3;
    }
    dc->SetWireframeMode( mode % 3 );
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
    case SDLK_UP:
        mUpArrowDown = true;
    break;
    case SDLK_DOWN:
        mDownArrowDown = true;
    break;
    case SDLK_LEFT:
        mLeftArrowDown = true;
    break;
    case SDLK_RIGHT:
        mRightArrowDown = true;
    break;
    case SDLK_HOME:
        mHomeDown = true;
    break;
    case SDLK_END:
        mEndDown = true;
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
    case SDLK_UP:
        mUpArrowDown = false;
    break;
    case SDLK_DOWN:
        mDownArrowDown = false;
    break;
    case SDLK_LEFT:
        mLeftArrowDown = false;
    break;
    case SDLK_RIGHT:
        mRightArrowDown = false;
    break;
    case SDLK_HOME:
        mHomeDown = false;
    break;
    case SDLK_END:
        mEndDown = false;
    break;
    default:
        break;
    }
}