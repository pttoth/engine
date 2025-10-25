#include "hunjam/Game.h"

#include "engine/actor/CameraPerspective.h"
#include "engine/MeshLoader.h"
#include "engine/Services.h"
#include "engine/service/SystemManager.h"

#include <thread>

using namespace engine;
using namespace math;

Game::
Game( const int argc, char* argv[] ):
    Engine( argc, argv )
{
    CfgAddKey( mGameCfg, strMediaURL );
    CfgAddKey( mGameCfg, strMediaHint );
    CfgAddKey( mGameCfg, bMoveableActor );
    CfgAddKey( mGameCfg, bMoveableSpotlight );
    CfgAddKey( mGameCfg, bCacoCloseup );
    CfgAddKey( mGameCfg, bShadowMapTesting );
    CfgAddKey( mGameCfg, bNormalVectorTesting );
    CfgAddKey( mGameCfg, bCirclingLights );
    CfgAddKey( mGameCfg, bPlasmaGunInHand );
    CfgAddKey( mGameCfg, bPreloadAllAssets );


    CfgAddKey( mMediaManifest, bHasRequiredMedia );
}


Game::
~Game()
{}


void Game::
OnStart()
{
    //--------------------------------------------------
    // Load config variables
    //   note: this runs before 'Engine::OnStart()' !
    //--------------------------------------------------
    // read config file
    std::string cfg_path = "../../cfg/OpenGL_test.cfg";
    try{
        PT_LOG_INFO( "Reading config file '" << cfg_path << "'." );
        mGameCfg.readF( cfg_path );
        mMediaURL               = mGameCfg.getS( strMediaURL );
        mMediaHint              = mGameCfg.getS( strMediaHint );
        mPreloadAllAssets       = mGameCfg.getB( bPreloadAllAssets );
        PT_LOG_INFO( "Successfully read config file '" << cfg_path << "'." );
    }catch( const std::exception& e ){
        PT_LOG_WARN( "Error with config file '" << cfg_path << "'!\n  " << e.what() );
    }catch(...){
        PT_LOG_WARN( "Unknown exception while handling config file '" << cfg_path << "'!" );
    }

    //--------------------------------------------------
    // Detect whether './media' directory is available
    //   note: this runs before 'Engine::OnStart()' !
    //--------------------------------------------------
    std::string manifest_path = "../../media/OpenGL_test.manifest.cfg";

    // failsafe for detecting, whether required media files are available
    try{
        mMediaManifest.readF( manifest_path );
        mHasRequiredMedia       = mMediaManifest.getB( bHasRequiredMedia );
        if( !mHasRequiredMedia ){
            throw std::logic_error( "missing media folder" );
        }
        PT_LOG_INFO( "Found media manifest." );
    }catch( const std::exception& e ){
        PT_LOG_INFO( "" );
        PT_LOG_INFO( "Could not find required media files!" );
        if( 0 == mMediaURL.length() ){
            PT_LOG_INFO( "  Wrong working directory at startup?" );
            PT_LOG_INFO( "  Try running with the startup script next to the binary." );
        }

        if( 0 < mMediaURL.length() ){
            std::stringstream ss;
            ss << "The files can be acquired here:"
                     "\n  URL:  " << mMediaURL;
            if( 0 < mMediaHint.length() ){
                ss << "\n  hint: '" << mMediaHint << "' ;)";

            }
            PT_LOG_INFO( ss.str() );
        }

        exit(1);
    }catch(...){
        PT_LOG_WARN( "Unknown exception while handling config file '" << manifest_path << "'!" );
        exit(1);
    }

    //--------------------------------------------------
    //  Engine::OnStart()
    //--------------------------------------------------
    Engine::OnStart();

    auto ac = Services::GetAssetControl();
    auto dc = Services::GetRenderer();

    // @TODO: do ./media search here, after reading the URL from the config
    // ...


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
    mMeshes.push_back( MeshEntry( "model/dev/dev_plasmaprojectile", gl::Mesh::FormatHint::GLTF ) );

    mSkyboxes.push_back( "texture/skybox/skybox_ocean1.png" );
    mSkyboxes.push_back( "texture/skybox/skybox_ocean_night1.png" );
    mSkyboxes.push_back( "texture/skybox/AndromedaDesertMako.png" );
    mSkyboxes.push_back( "texture/skybox/sky_over_clouds1.png" );
    mSkyboxes.push_back( "texture/skybox/SpaceMeteorField1.png" );
    mSkyboxes.push_back( "texture/skybox/desert_cloudy_day1.png" );
    mSkyboxes.push_back( "texture/skybox/overcast_soil_puresky_2k.png" );
    mSkyboxes.push_back( "texture/skybox/scythian_tombs_puresky_2k.png" );
    mSkyboxes.push_back( "texture/skybox/sunflowers_puresky_2k.png" );


    if( mPreloadAllAssets ){
        // preload skybox textures (slows down startup too much)
        for( auto& s : mSkyboxes ){
            ac->LoadTexture( s );
        }

        // preload textures (slows down startup too much)
        for( auto& t : mTextures ){
            ac->LoadTexture( t );
        }

        // preload materials
        for( auto e : mMaterials ){
            ac->LoadMaterial( e );
        }
    }

    // preload meshes
    for( auto e : mMeshes ){
        // NOTE: this is mandatory for now, because late-fetching cannot deduce the MeshFormat hint!
        //  late-fetching GLTF crashes, preloading with hint prevents it
        ac->LoadMesh( e.mName, e.mHint );
    }


    // -------------------------
    // set up camera
    auto camera = engine::Services::GetRenderer()->GetMainCamera();

    camera->SetAspectRatio( 16.0f / 9.0f );
    camera->SetFOVDeg( 75.0f );
    camera->SetPosition( vec3( 1500.0f, 1500.0f, 500.0f ) );
    camera->LookAt( vec3::zero ); // look at origo


    // -------------------------
    // set up origo axis display

    mWorldAxis = NewPtr<WorldAxisActor>( "mWorldAxis" );
    mWorldAxis->SetScale( vec3::one * 100000.0f );
    mWorldAxis->CreateRenderContext();
    mWorldAxis->Spawn();
    Actor::RegisterTickFunction( mWorldAxis );

    // -------------------------
    // set up skybox
    dc->SetSkyboxTexture( mSkyboxes[mCurrentSkyboxIndex] );
    dc->SetWireframeMode( 0 );

    EnableFreeLook( true );

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

}


void Game::
UpdateGameState_PostActorTick( float t, float dt )
{
    auto camera = engine::Services::GetRenderer()->GetMainCamera();

    // reset mouse position to middle of window
    if( HasKeyboardFocus() && mFreeLook ){
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
OnEvent( SDL_Event* event )
{
    Engine::OnEvent( event );

    SDL_Event ev = *event;
    if( SDL_WINDOWEVENT == ev.type ){
        switch( ev.window.event ){
        case SDL_WINDOWEVENT_ENTER:
            break;
        case SDL_WINDOWEVENT_LEAVE:
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            if( mFreeLook ){
                SDL_SetRelativeMouseMode( SDL_TRUE );   // hide mouse cursor
            }
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            if( mFreeLook ){
                SDL_SetRelativeMouseMode( SDL_FALSE );  // restore mouse cursor display
            }
            break;
        }
    }
}


void Game::
OnMouseButtonDown(int32_t x, int32_t y,
                  uint8_t button, uint8_t clicks,
                  uint32_t timestamp, uint32_t mouseid)
{
    if( button == SDL_BUTTON_LEFT ){

    }else if( button == SDL_BUTTON_RIGHT ){

        if( mSkyboxSelectionActive ){
            auto dc = Services::GetRenderer();
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
    if( button == SDL_BUTTON_LEFT ){
    }
}


void Game::
OnMouseMotion(int32_t x, int32_t y,
              int32_t x_rel, int32_t y_rel,
              uint32_t timestamp, uint32_t mouseid)
{
    if( HasKeyboardFocus() && mFreeLook ){
        static float mousespeed_x = 0.30f;
        static float mousespeed_y = 0.30f;

        auto camera = engine::Services::GetRenderer()->GetMainCamera();

        // @TODO: only collect mouse motion data here,
        //          don't reorient the camera for every event
        //        mousemotion report rate can be 500-1000 times per sec, or more, way too many times per frame
        //180 pixel = 30 degree = pi/6
        camera->RotateCamera( y_rel * mousespeed_y /180 * static_cast<float>(M_PI) / 6,
                              x_rel * mousespeed_x /180 * static_cast<float>(M_PI) / 6 );
    }
}


void Game::
OnMouseWheel( int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction )
{
    auto dc = Services::GetRenderer();
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

    default:
        break;
    }

}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod,
        uint32_t timestamp, uint8_t repeat)
{
    switch( keycode ){
    default:
        break;
    }
}


void Game::
EnableFreeLook( bool value )
{
    if( value ){
        if( HasKeyboardFocus() ){
            SDL_SetRelativeMouseMode( SDL_TRUE );   // hide mouse cursor
        }
    }else{
        SDL_SetRelativeMouseMode( SDL_FALSE );      // show mouse cursor
    }

    mFreeLook = value;
}
