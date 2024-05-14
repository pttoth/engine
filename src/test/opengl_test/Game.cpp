#include "test/opengl_test/Game.h"

#include "engine/MeshLoader.h"
#include "engine/Services.h"
#include "engine/service/SystemManager.h"


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

    auto ac = Services::GetAssetControl();
    auto dc = Services::GetDrawingControl();

    mMeshes.push_back( "model/doom3/models/md5/monsters/cacodemon/cacodemon" );
    mMeshes.push_back( "model/campbell/campbell" );

    mSkyboxes.push_back( "texture/skybox/skybox_ocean1.png" );
    mSkyboxes.push_back( "texture/skybox/skybox_ocean_night1.png" );
    mSkyboxes.push_back( "texture/skybox/skybox_cloudy_desert1.png" );
    mSkyboxes.push_back( "texture/skybox/AndromedaDesertMako.png" );
    mSkyboxes.push_back( "texture/skybox/SpaceMeteorField1.png" );
    mSkyboxes.push_back( "texture/skybox/bay_dusk1.png" );
    mSkyboxes.push_back( "texture/skybox/desert_cloudy_day1.png" );
    mSkyboxes.push_back( "texture/skybox/view-from-the-balcony-to-the-green-city-on-a-sunny-day-R1FBYH.png" );


    mBillboardTexture = NewPtr<engine::gl::Texture2d>("mBillboardTexture");
    mBillboardTexture->ReadFilePNG( "../../media/texture/Blade512.png" );
    //mBillboardTexture->ReadFilePNG( "../../media/texture/doom3/marine/marine.png" );

    mBillboardTexture->LoadToVRAM();

    mBillboardActor.CreateRenderContext();
    mBillboardActor.SetTexture( mBillboardTexture );
    mBillboardActor.SetMesh( mMeshes[mCurrentSkyboxIndex] );
    mBillboardActor.Spawn();
    Actor::RegisterTickFunction( mBillboardActor );

    // preload textures and meshes (slows down startup too much)
    /*
    for( auto& m : mMeshes ){
        ac->LoadMesh( m );
    }
    for( auto& s : mSkyboxes ){
        ac->LoadTexture( s );
    }
    */

    dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );

    auto shp = dc->GetDefaultShaderProgram();
    dc->SetWireframeMode( 0 );

    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();
    camera->SetPosition( vec3( 1500.0f, 1500.0f, 500.0f ) );
    camera->LookAt( vec3::zero ); // look at origo
}


void Game::
OnExit()
{

    Engine::OnExit();
}


void Game::
UpdateGameState_PreActorTick( float t, float dt )
{
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
    bool doRotate = false;
    float rotSpeed = (180)* dt / 4;

    if ( mRotXDown ){
        rotX += rotSpeed;
        doRotate = true;
    }
    if ( mRotX_Down ){
        rotX -= rotSpeed;
        doRotate = true;
    }
    if ( mRotYDown ){
        rotY += rotSpeed;
        doRotate = true;
    }
    if ( mRotY_Down ){
        rotY -= rotSpeed;
        doRotate = true;
    }
   if ( mRotZDown ){
        rotZ += rotSpeed;
        doRotate = true;
    }
    if ( mRotZ_Down ){
        rotZ -= rotSpeed;
        doRotate = true;
    }

    if( doRotate ){
        mat4 tf = mBillboardActor.GetRelativeTransform();
        mBillboardActor.SetRelativeTransform( tf * FRotator( rotX, rotY, rotZ ).GetTransform() );
    }
}


void Game::
UpdateGameState_PostActorTick( float t, float dt )
{
    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();
    float cameraBaseSpeed = 1000.f;
    float cameraSpeedMultiplier = 1.0f/3;
    float cameraSpeed = cameraBaseSpeed * dt;

    if (mShiftDown){
        cameraSpeed = cameraBaseSpeed * cameraSpeedMultiplier * dt;
    }

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


    float PawnSpeed = 300.0f * dt;
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
    //mBillboardActor.SetOrientation( Xrot );
}


void Game::
OnMouseButtonDown(int32_t x, int32_t y,
                  uint8_t button, uint8_t clicks,
                  uint32_t timestamp, uint32_t mouseid)
{
    mLMBDown = true;
    if( button == SDL_BUTTON_LEFT ){
        if( mFreeLook ){
            SDL_SetRelativeMouseMode(SDL_FALSE);
        }else{
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }
        mFreeLook = !mFreeLook;
    }else if( button == SDL_BUTTON_RIGHT ){
        if( mSkyboxSelectionActive ){
            auto dc = Services::GetDrawingControl();
            if( mSkyboxEnabled ){
                if( nullptr != dc ){
                    dc->SetSkyboxTexture( "" );
                }
                mSkyboxEnabled = false;
            }else{
                if( nullptr != dc ){
                    dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
                }
                mSkyboxEnabled = true;
            }

        }
    }
}


void Game::
OnMouseButtonUp(int32_t x, int32_t y,
                uint8_t button, uint8_t clicks,
                uint32_t timestamp, uint32_t mouseid)
{
    mLMBDown = false;
}


void Game::
OnMouseMotion(int32_t x, int32_t y,
              int32_t x_rel, int32_t y_rel,
              uint32_t timestamp, uint32_t mouseid)
{
    if( mFreeLook ){
        static float mousespeed_x = 0.30f;
        static float mousespeed_y = 0.30f;

        auto camera = engine::Services::GetDrawingControl()->GetMainCamera();

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

    if( mSkyboxSelectionActive ){
        size_t size = mSkyboxes.size();
        if( 0 < y ){
            mCurrentSkyboxIndex = (mCurrentSkyboxIndex-1+size) %size;
        }else{
            mCurrentSkyboxIndex = (mCurrentSkyboxIndex+1) %size;
        }
        dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
    }else if( mMeshSelectionActive ){
        size_t size = mMeshes.size();
        if( 0 < y ){
            mCurrentMeshIndex = (mCurrentMeshIndex-1+size) %size;
        }else{
            mCurrentMeshIndex = (mCurrentMeshIndex+1) %size;
        }
        mBillboardActor.SetMesh( mMeshes[mCurrentMeshIndex] );
    }else{
        if( 0 < y ){
            mode = (mode-1+3) %3;
        }else{
            mode = (mode+1) %3;
        }
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
    case SDLK_LSHIFT:
        mShiftDown = true;
    break;

    case SDLK_b:
        mSkyboxSelectionActive = true;
        break;
    case SDLK_m:
        mMeshSelectionActive = true;
        break;

    case SDLK_r:
        mRotationMode = not mRotationMode;
        //mRotationMode = true;
        break;

    case SDLK_u:
        mRotXDown = true;
        break;
    case SDLK_j:
        mRotX_Down = true;
        break;
    case SDLK_i:
        mRotYDown = true;
        break;
    case SDLK_k:
        mRotY_Down = true;
        break;
    case SDLK_o:
        mRotZDown = true;
        break;
    case SDLK_l:
        mRotZ_Down = true;
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
    case SDLK_LSHIFT:
        mShiftDown= false;
    break;

    case SDLK_b:
        mSkyboxSelectionActive = false;
        break;
    case SDLK_m:
        mMeshSelectionActive = false;
        break;

/*
    case SDLK_r:
        mRotationMode = false;
        break;
*/
    case SDLK_u:
        mRotXDown = false;
        break;
    case SDLK_j:
        mRotX_Down = false;
        break;
    case SDLK_i:
        mRotYDown = false;
        break;
    case SDLK_k:
        mRotY_Down = false;
        break;
    case SDLK_o:
        mRotZDown = false;
        break;
    case SDLK_l:
        mRotZ_Down = false;
        break;




    default:
        break;
    }
}
