#include "test/opengl_test/Game.h"

#include "test/opengl_test/WorldGeometry.h"

#include "engine/actor/CameraPerspective.h"
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
{
    CfgAddKey( mGameCfg, bCacoCloseup );
}


Game::
~Game()
{}


void Game::
OnStart()
{
    Engine::OnStart();

    auto ac = Services::GetAssetControl();
    auto dc = Services::GetDrawingControl();

    std::string cfg_path = "../../cfg/OpenGL_test.cfg";

    try{
        PT_LOG_INFO( "Reading config file '" << cfg_path << "'." );
        mGameCfg.readF( cfg_path );
        mCacoCloseup = mGameCfg.getB( bCacoCloseup );
        PT_LOG_INFO( "Successfully read config file '" << cfg_path << "'." );
    }catch( const std::exception& e ){
        PT_LOG_WARN( "Error with config file '" << cfg_path << "'!\n  " << e.what() );
    }catch(...){
        PT_LOG_WARN( "Unknown exception while handling config file '" << cfg_path << "'!" );
    }

    mMeshes.push_back( MeshEntry( "dev_camera", gl::Mesh::FormatHint::GLTF ) );

    // WARNING: when using non-default (MD5_IDTECH4) formats, meshes have to be pre-loaded
    //          the late-fetching logic cannot yet deduce the mesh format and assumes 'MD5_IDTECH4'
    mMeshes.push_back( MeshEntry( "model/doom3/models/md5/monsters/cacodemon/cacodemon" ) );
    mMeshes.push_back( MeshEntry( "model/campbell/campbell" ) );
    mMeshes.push_back( MeshEntry( "model/doom3/models/md5/weapons/plasmagun_view/viewplasmagun" ) );

    // crashes! debug!
    //mMeshes.push_back( MeshEntry( "cube2", gl::Mesh::FormatHint::GLTF ) );
    //mMeshes.push_back( MeshEntry( "map1_v1", gl::Mesh::FormatHint::GLTF ) );
    //mMeshes.push_back( MeshEntry( "map1_v2", gl::Mesh::FormatHint::GLTF ) );

    mMeshes.push_back( MeshEntry( "model/dev/testmap1/pillar1", gl::Mesh::FormatHint::GLTF ) );
    mMeshes.push_back( MeshEntry( "model/dev/testmap1/pavement1", gl::Mesh::FormatHint::GLTF ) );
    mMeshes.push_back( MeshEntry( "model/dev/testmap1/wall1", gl::Mesh::FormatHint::GLTF ) );
    mMeshes.push_back( MeshEntry( "dev_camera", gl::Mesh::FormatHint::GLTF ) );

    mSkyboxes.push_back( "texture/skybox/skybox_ocean1.png" );
    mSkyboxes.push_back( "texture/skybox/skybox_ocean_night1.png" );
    mSkyboxes.push_back( "texture/skybox/AndromedaDesertMako.png" );
    mSkyboxes.push_back( "texture/skybox/sky_over_clouds1.png" );
    mSkyboxes.push_back( "texture/skybox/SpaceMeteorField1.png" );
    mSkyboxes.push_back( "texture/skybox/desert_cloudy_day1.png" );
    mSkyboxes.push_back( "texture/skybox/overcast_soil_puresky_2k.png" );
    mSkyboxes.push_back( "texture/skybox/scythian_tombs_puresky_2k.png" );
    mSkyboxes.push_back( "texture/skybox/sunflowers_puresky_2k.png" );
    //mSkyboxes.push_back( "texture/skybox/sunflowers_puresky_8k.png" );
    //mSkyboxes.push_back( "texture/skybox/skybox_cloudy_desert1.png" );
    //mSkyboxes.push_back( "texture/skybox/fouriesburg_mountain_cloudy_16k.png" );

    //mSkyboxes.push_back( "texture/skybox/bay_dusk1.png" );

    //mSkyboxes.push_back( "texture/skybox/view-from-the-balcony-to-the-green-city-on-a-sunny-day-R1FBYH.png" );
    //mSkyboxes.push_back( "texture/skybox/citrus_orchard_road_2k.png" );
    //mSkyboxes.push_back( "texture/skybox/citrus_orchard_road_8k.png" );
    //mSkyboxes.push_back( "texture/skybox/citrus_orchard_road_16k.png" );
    //mSkyboxes.push_back( "texture/skybox/citrus_orchard_road_20k.png" );  //crashes
    //mSkyboxes.push_back( "texture/skybox/evening_road_01_puresky_8k.png" );
    //mSkyboxes.push_back( "texture/skybox/evening_road_01_puresky_16k.png" );

    //mSkyboxes.push_back( "texture/skybox/kloofendal_48d_partly_cloudy_puresky_16k.png" );


    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/plasmagun_ventglow" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/plasmagun_mflash2" );



    mMaterials.push_back( "material/dev/dev_measuregeneric01" );
    mMaterials.push_back( "material/dev/dev_measuregeneric01b" );
    mMaterials.push_back( "material/doom3/models/characters/male_npc/marine/marine" );
    mMaterials.push_back( "material/doom3/models/characters/player/arm2" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacobrain" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacodemon" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacodemon_mouth" );
    mMaterials.push_back( "material/doom3/models/weapons/bfg/bfg_world" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p1" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p1x" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p2" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p2x" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p3" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/p3x" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/plasmagun_mflash" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/pl_can" );
    mMaterials.push_back( "material/doom3/models/weapons/plasmagun/pl_can_blue" );
    mMaterials.push_back( "material/doom3/textures/common/entityGui" );
    mMaterials.push_back( "material/doom3/textures/common/shadow" );

    mMaterials.push_back( "material/doom3/models/characters/male_npc/marine/marine" );
    mMaterials.push_back( "material/doom3/models/characters/player/arm2" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacobrain" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacodemon" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacodemon_mouth" );
    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacoeye" );
    mMaterials.push_back( "material/doom3/models/weapons/bfg/bfg_world" );
    mMaterials.push_back( "material/doom3/textures/common/entityGui" );
    mMaterials.push_back( "material/doom3/textures/common/shadow" );

    //mMaterials.push_back( "models/weapons/plasmagun/plasmagun" );
    //mTextures.push_back( "models/weapons/plasmagun/plasmagun" );

    mTextures.push_back( "texture/dev/dev_measuregeneric01b.png" );
    mTextures.push_back( "texture/dev/dev_measuregeneric01.png" );
    mTextures.push_back( "texture/doom3/models/characters/male_npc/marine/marine.png" );
    mTextures.push_back( "texture/doom3/models/monsters/cacodemon/cacobrain.png" );
    mTextures.push_back( "texture/doom3/models/monsters/cacodemon/cacodemon_d.png" );
    mTextures.push_back( "texture/doom3/models/monsters/cacodemon/cacodemon_s.png" );
    mTextures.push_back( "texture/doom3/models/monsters/cacodemon/cacoeye.png" );
    mTextures.push_back( "texture/doom3/models/weapons/bfg/bfg_world.png" );
    mTextures.push_back( "texture/skybox/skybox_ocean1.png" );



    mMaterials.push_back( "material/doom3/models/monsters/cacodemon/cacoeye" );




    // preload skybox textures (slows down startup too much)
    /*
    for( auto& s : mSkyboxes ){
        ac->LoadTexture( s );
    }
*/

    // preload textures (slows down startup too much)

    for( auto& t : mTextures ){
        ac->LoadTexture( t );
    }


    // preload materials

    for( auto e : mMaterials ){
        ac->LoadMaterial( e );
    }


    // preload meshes
    for( auto e : mMeshes ){
        // NOTE: this is mandatory for now, because late-fetching cannot deduce the MeshFormat hint!
        //  late-fetching GLTF crashes, preloading with hint prevents it
        ac->LoadMesh( e.mName, e.mHint );
    }







    vec3 billActorPos = vec3( 0, 0, 1000.0f );
    mBillboardTexture = gl::Texture2d::CreateFromPNG( "mBillboardTexture", "../../media/texture/Blade512.png" );
    mBillboardTexture->LoadToVRAM();

    mBillboardActor.CreateRenderContext();
    mBillboardActor.SetTexture( mBillboardTexture );
    mBillboardActor.SetMesh( mMeshes[mCurrentSkyboxIndex].mName );
    mBillboardActor.SetPosition( billActorPos );
    mBillboardActor.Spawn();
    Actor::RegisterTickFunction( mBillboardActor );






    dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
    dc->SetWireframeMode( 0 );
    // -------------------------
    // set up map layout

    mWorldGeometry = NewPtr<WorldGeometry>( "WorldGeometry" );
    mWorldGeometry->CreateRenderContext();
    mWorldGeometry->Spawn();
    mWorldGeometry->SetPosition( vec3( 0, 0, -10000.0f ) ); // @TODO: doesn't work for some reason
    Actor::RegisterTickFunction( mWorldGeometry );

    //mWorldGeometry->SetScale( 1000 );

    // -------------------------
    auto camera = engine::Services::GetDrawingControl()->GetMainCamera();

    camera->SetAspectRatio( 16.0f / 9.0f );
    camera->SetFOVDeg( 75.0f );
    camera->SetPosition( vec3( 1500.0f, 1500.0f, 500.0f ) );
    camera->LookAt( vec3::zero ); // look at origo

    mWorldAxis = NewPtr<WorldAxisActor>( "mWorldAxis" );
    mWorldAxis->SetScale( vec3::one * 100000.0f );
    mWorldAxis->CreateRenderContext();
    mWorldAxis->Spawn();

    Actor::RegisterTickFunction( mWorldAxis );

    mLightConeActor = NewPtr<LightCone>( "LightConeActor" );
    Actor::RegisterTickFunction( mLightConeActor );
    //mLightConeActor->SetParent( mBillboardActor );
    mLightConeActor->SetPosition( vec3( 0.0f, 0.0f, 200.0f ) );
    mLightConeActor->CreateRenderContext();
    mLightConeActor->Spawn();

    // caco closeup
    bool cacoCloseup = mCacoCloseup;
    //cacoCloseup = false;
    //cacoCloseup = true;
    if( cacoCloseup ){
        camera->SetPosition( vec3( 850.0f, 0.0f, 1000.0f ) );
        camera->LookAt( billActorPos );
        mLightConeActor->SetPosition( vec3( 250.0f, 0.0f, 200.0f ) );
        mLightConeActor->SetRotation( FRotator( -90, 0, 180 ) ); // face camera upwards
                                                    // @TODO: pitch should be positive upwards, no?
    }

}


void Game::
OnExit()
{
    /*
    mBillboardTexture->FreeVRAM();
    mBillboardTexture->FreeClientsideData();
    mBillboardActor.DestroyRenderContext();
    mWorldAxis->DestroyRenderContext();
    */
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

        if( !mCacoCloseup ){
            mat4 tfl = mLightConeActor->GetRelativeTransform();
            mLightConeActor->SetRelativeTransform( tfl * FRotator( rotX, rotY, rotZ ).GetTransform() );
        }
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
        movedir += camera->GetDir( engine::Camera::Dir::FORWARD ).XYZ();
        cameramoved = true;
    }
    if (mBackDown){
        movedir += camera->GetDir( engine::Camera::Dir::BACKWARD ).XYZ();
        cameramoved = true;
    }
    if (mLeftDown){
        movedir += camera->GetDir( engine::Camera::Dir::LEFT ).XYZ();
        cameramoved = true;
    }
    if (mRightDown){
        movedir += camera->GetDir( engine::Camera::Dir::RIGHT ).XYZ();
        cameramoved = true;
    }
    if (mAscendDown){
        movedir += camera->GetDir( engine::Camera::Dir::UP ).XYZ();
        cameramoved = true;
    }
    if (mDescendDown){
        movedir += camera->GetDir( engine::Camera::Dir::DOWN ).XYZ();
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
        pawnMoveDir += vec3::xUnit;
        pawnMoved = true;
    }
    if (mDownArrowDown){
        pawnMoveDir += vec3::xUnit * -1;
        pawnMoved = true;
    }
    if (mLeftArrowDown){
        pawnMoveDir += vec3::yUnit;
        pawnMoved = true;
    }
    if (mRightArrowDown){
        pawnMoveDir += vec3::yUnit * -1;
        pawnMoved = true;
    }
    if (mPageUp_Down){
        pawnMoveDir += vec3::zUnit;
        pawnMoved = true;
    }
    if (mPageDown_Down){
        pawnMoveDir += vec3::zUnit * -1;
        pawnMoved = true;
    }

    if (mHomeDown){
        camera->LookAt( vec3::zero );
    }
    if( mEndDown){
        camera->LookAt( mBillboardActor.GetWorldPosition() );
    }


    if( pawnMoved ){
        if( 0.0001f < pawnMoveDir.length()  ){
            vec3 pos = mBillboardActor.GetPosition();
            mBillboardActor.SetPosition( pos + pawnMoveDir.normalize() * PawnSpeed );

            if( !mCacoCloseup ){
                vec3 posl = mLightConeActor->GetPosition();
                mLightConeActor->SetPosition( posl + pawnMoveDir.normalize() * PawnSpeed );
            }
        }
    }

    FRotator Xrot( 2.5f *t, 0, 0 );
    //mBillboardActor.SetOrientation( Xrot );

    if( mFreeLook ){
        auto ec = Services::GetEngineControl();
        math::int2 dimWindow = ec->GetMainWindowDimensions();
        math::int2 posWindow = ec->GetMainWindowPosition();
        int w = dimWindow.x;
        int h = dimWindow.y;
        int x = posWindow.x;
        int y = posWindow.x;

        SDL_WarpMouseGlobal( x+w/2, y+h/2 );
    }
}


void Game::
OnMouseButtonDown(int32_t x, int32_t y,
                  uint8_t button, uint8_t clicks,
                  uint32_t timestamp, uint32_t mouseid)
{
    mLMBDown = true;
    if( button == SDL_BUTTON_LEFT ){
        if( mFreeLook ){
            SDL_SetRelativeMouseMode( SDL_FALSE );
        }else{
            SDL_SetRelativeMouseMode( SDL_TRUE );
        }
        mFreeLook = !mFreeLook;
    }else if( button == SDL_BUTTON_RIGHT ){
        if( mSkyboxSelectionActive ){
            auto dc = Services::GetDrawingControl();
            mSkyboxEnabled = !mSkyboxEnabled;
            dc->EnableSkybox( mSkyboxEnabled );
            if( mSkyboxEnabled ){
                dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
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
        PT_LOG_INFO( "Skybox '" << mSkyboxes[mCurrentSkyboxIndex] << "' selected." );
        if( mSkyboxEnabled ){
            dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
        }
    }else if( mMeshSelectionActive ){
        size_t size = mMeshes.size();
        if( 0 < y ){
            mCurrentMeshIndex = (mCurrentMeshIndex-1+size) %size;
        }else{
            mCurrentMeshIndex = (mCurrentMeshIndex+1) %size;
        }
        mBillboardActor.SetMesh( mMeshes[mCurrentMeshIndex].mName );
    }else if( mFovSelectionActive ){
        auto cam = dc->GetMainCamera();
        CameraPerspective* cp = dynamic_cast<CameraPerspective*>( cam.get() );
        if( nullptr == cp ){
            return;
        }
        if( 0 < y ){
            --mFoVAdjustment;
        }else{
            ++mFoVAdjustment;
        }
        float newfov = mDefaultFoV + mFoVAdjustment;
        cp->SetFOVDeg( newfov );
        PT_LOG_DEBUG( "camera FoV: " << newfov );
    }else if( mLightAngleSelectionActive ){
        if( 0 < y ){
            mLightAngle = mLightAngle - 1.0f;
        }else{
            mLightAngle = mLightAngle + 1.0f;
        }
        mLightConeActor->SetAngle( mLightAngle );

        PT_LOG_DEBUG( "light angle: " << mLightAngle );



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
    case SDLK_PAGEUP:
        mPageUp_Down = true;
    break;
    case SDLK_PAGEDOWN:
        mPageDown_Down = true;
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
    case SDLK_v:
        mFovSelectionActive = true;
        break;
    case SDLK_f:
        mLightAngleSelectionActive = true;
        break;

    case SDLK_r:
        mRotationMode = not mRotationMode;
        //mRotationMode = true;
        break;

    case SDLK_u:
        mRotZDown = true;
        break;
    case SDLK_j:
        mRotYDown = true;
        break;
    case SDLK_i:
        mRotX_Down = true;
        break;
    case SDLK_k:
        mRotXDown = true;
        break;
    case SDLK_o:
        mRotZ_Down = true;
        break;
    case SDLK_l:
        mRotY_Down = true;
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
    case SDLK_PAGEUP:
        mPageUp_Down = false;
    break;
    case SDLK_PAGEDOWN:
        mPageDown_Down = false;
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
    case SDLK_v:
        mFovSelectionActive = false;
        break;
    case SDLK_f:
        mLightAngleSelectionActive = false;
        break;

/*
    case SDLK_r:
        mRotationMode = false;
        break;
*/
    case SDLK_u:
        mRotZDown = false;
        break;
    case SDLK_j:
        mRotYDown = false;
        break;
    case SDLK_i:
        mRotX_Down = false;
        break;
    case SDLK_k:
        mRotXDown = false;
        break;
    case SDLK_o:
        mRotZ_Down = false;
        break;
    case SDLK_l:
        mRotY_Down = false;
        break;




    default:
        break;
    }
}
