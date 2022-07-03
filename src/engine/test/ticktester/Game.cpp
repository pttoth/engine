#include "engine/test/ticktester/Game.h"

#include "engine/Services.h"

#include "pt/logging.h"

#include <algorithm>
#include <sstream>


using namespace test;
using namespace test::ticktester;

using namespace engine;
using namespace pt::math;


Game::
Game():
    mCamera("MainCamera"),
    mPlayerPawn("PlayerPawn")
{
    std::stringstream ss;
    ss << pt::log::AutoGenerateLogFileName();

    pt::log::Initialize("./", ss.str());

    mTickrateTable.reserve(32);
    mTickrateTable.push_back( 0.5f );   // 1/2 fps
    mTickrateTable.push_back( 1.0f );   // 1 fps
    mTickrateTable.push_back( 2.0f );   // 2 fps
    mTickrateTable.push_back( 4.0f );   //...
    mTickrateTable.push_back( 8.0f );
    mTickrateTable.push_back( 12.0f );
    mTickrateTable.push_back( 15.0f );
    mTickrateTable.push_back( 20.0f );

    mTickrateTable.push_back( 24.0f );
    mTickrateTable.push_back( 25.0f );
    mTickrateTable.push_back( 30.0f );
    mTickrateTable.push_back( 40.0f );
    mTickrateTable.push_back( 45.0f );
    mTickrateTable.push_back( 50.0f );  // idx 13
    mTickrateTable.push_back( 55.0f );
    mTickrateTable.push_back( 60.0f );

    mTickrateTable.push_back( 65.0f );
    mTickrateTable.push_back( 70.0f );
    mTickrateTable.push_back( 75.0f );
    mTickrateTable.push_back( 80.0f );
    mTickrateTable.push_back( 85.0f );
    mTickrateTable.push_back( 90.0f );
    mTickrateTable.push_back( 95.0f );
    mTickrateTable.push_back( 100.0f );

    mTickrateTable.push_back( 105.0f );
    mTickrateTable.push_back( 110.0f );
    mTickrateTable.push_back( 115.0f );
    mTickrateTable.push_back( 120.0f );
    mTickrateTable.push_back( 144.0f );
}


Game::
~Game()
{
    if(mInitialized){
        DestroyContext();
    }
}


void Game::
OnStart()
{
    Engine::OnStart();

    InitContext();

    //Services::GetSDLControl()->SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_NONE);
    //Services::GetSDLControl()->SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_ADD);
    Services::GetSDLControl()->SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
    //Services::GetSDLControl()->SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_MOD);

    Services::GetDrawingControl()->SetMainCamera(&mCamera);
    float3 pos;
    float4 ori;
    //ori = float4(0.0f, 0.0f, -1.0f, 1.0f);
    mCamera.GetRootComponent()->SetPosition(pos);
    mCamera.GetRootComponent()->SetOrientation(ori);


    //initialize entities
    Entity::RegisterTickFunction(&mPlayerPawn, Ticker::Group::PREPHYSICS); //TODO: move this to PlayerPawn


    mPlayerPawn.Spawn();
    //mListeners.push_back(&mPlayerPawn);
}


void Game::
OnExit()
{
    //------------
    //code here...

    Services::GetDrawingControl()->SetMainCamera(nullptr);

    if(mInitialized){
        DestroyContext();
    }
    //------------
    Engine::OnExit();
}


void Game::
OnShutdownSignal()
{
    //------------
    //code here...


    //------------
    Engine::OnShutdownSignal();
}


void Game::
Update(float t, float dt)
{
    //Engine::tick(t, dt);

    auto rootComp = mPlayerPawn.GetRootComponent();
    auto bbc = mPlayerPawn.getSubRect();

    float mainSpeed = 2.0f;
    float subSpeed = 2.0f;

    ColorRectComponent::ResetLastID();

    {
        bool updateMain = mButtonPressedMoveMainUp
                          ||mButtonPressedMoveMainDown
                          ||mButtonPressedMoveMainLeft
                          ||mButtonPressedMoveMainRight;
        float3 updateMainVec;

        if(mButtonPressedMoveMainUp){
            updateMainVec.y += mainSpeed * dt;
        }
        if(mButtonPressedMoveMainDown){
            updateMainVec.y -= mainSpeed * dt;
        }
        if(mButtonPressedMoveMainLeft){
            updateMainVec.x -= mainSpeed * dt;
        }
        if(mButtonPressedMoveMainRight){
            updateMainVec.x += mainSpeed * dt;
        }
        if(updateMain){
            float3 pos = rootComp->GetPosition();
            rootComp->SetPosition(pos + updateMainVec);
        }
    }

    {
        bool updateSub = mButtonPressedMoveSubUp
                         ||mButtonPressedMoveSubDown
                         ||mButtonPressedMoveSubLeft
                         ||mButtonPressedMoveSubRight;
        float3 updateSubVec;

        if(mButtonPressedMoveSubUp){
            updateSubVec.y += subSpeed * dt;
        }
        if(mButtonPressedMoveSubDown){
            updateSubVec.y -= subSpeed * dt;
        }
        if(mButtonPressedMoveSubLeft){
            updateSubVec.x -= subSpeed * dt;
        }
        if(mButtonPressedMoveSubRight){
            updateSubVec.x += subSpeed * dt;
        }

        if(updateSub){
            float3 pos_bbc = bbc->GetPosition();
            bbc->SetPosition(pos_bbc + updateSubVec);
        }
    }

    {
        mDeltaRadius = 0.0f;
        if(mButtonPressedIncreaseRadius){
            mDeltaRadius += 1.0f;
        }
        if(mButtonPressedDecreaseRadius){
            mDeltaRadius -= 1.0f;
        }

        mPlayerPawn.SetFloatRadius( mPlayerPawn.GetFloatRadius() + mDeltaRadius * dt );
    }

    auto dc = Services::GetDrawingControl();
    dc->DrawScene(t, dt);
}


void Game::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


template<typename T>
const T& clamp( const T& v, const T& lo, const T& hi ){
    return std::min( std::max( lo, v ), hi );
}

void Game::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    int32_t rate = 0;
    switch(keycode){
    case SDLK_w:
        mButtonPressedMoveMainUp = true;
        break;
    case SDLK_s:
        mButtonPressedMoveMainDown = true;
        break;
    case SDLK_a:
        mButtonPressedMoveMainLeft = true;
        break;
    case SDLK_d:
        mButtonPressedMoveMainRight = true;
        break;
    case SDLK_u:
        mButtonPressedMoveSubUp = true;
        break;
    case SDLK_j:
        mButtonPressedMoveSubDown = true;
        break;
    case SDLK_h:
        mButtonPressedMoveSubLeft = true;
        break;
    case SDLK_k:
        mButtonPressedMoveSubRight = true;
        break;
    case SDLK_q:
        mButtonPressedIncreaseRadius = true;
        break;
    case SDLK_e:
        mButtonPressedDecreaseRadius = true;
        break;
    case SDLK_KP_PLUS:
        mTickrateTableIdx = clamp(mTickrateTableIdx+1, (size_t) 0, mTickrateTable.size() -1) ;
        this->SetTickrate( mTickrateTable[mTickrateTableIdx] );
        break;
    case SDLK_KP_MINUS:
        ++mTickrateTableIdx; //to avoid unsigned underflow
        mTickrateTableIdx = clamp(mTickrateTableIdx-1, (size_t) 1, mTickrateTable.size()) -1;
        this->SetTickrate( mTickrateTable[mTickrateTableIdx] );
        break;
    }
}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    switch(keycode){
    case SDLK_w:
        mButtonPressedMoveMainUp = false;
        break;
    case SDLK_s:
        mButtonPressedMoveMainDown = false;
        break;
    case SDLK_a:
        mButtonPressedMoveMainLeft = false;
        break;
    case SDLK_d:
        mButtonPressedMoveMainRight = false;
        break;
    case SDLK_u:
        mButtonPressedMoveSubUp = false;
        break;
    case SDLK_j:
        mButtonPressedMoveSubDown = false;
        break;
    case SDLK_h:
        mButtonPressedMoveSubLeft = false;
        break;
    case SDLK_k:
        mButtonPressedMoveSubRight = false;
        break;
    case SDLK_q:
        mButtonPressedIncreaseRadius = false;
        break;
    case SDLK_e:
        mButtonPressedDecreaseRadius = false;
        break;
    }
}

void Game::
InitContext()
{
    InitSdlService();

    SDLControl* sdl = Services::GetSDLControl();

    int init = SDL_Init( SDL_INIT_EVENTS
                         | SDL_INIT_TIMER
                         | SDL_INIT_AUDIO
                         | SDL_INIT_VIDEO
                         );

    if( 0 != init ){
        throw std::runtime_error("Failed to initialize SDL");
    }

    //initialize window
    uint32_t pxWidth = 1280;
    uint32_t pxHeight = 720;

    //pxWidth = 960;
    //pxHeight = 960;

    uint32_t flags = 0;

    mWindow = sdl->CreateWindow("Tick logic test",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                pxWidth, pxHeight, flags);
    mRenderer = sdl->CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    sdl->SetMainWindow(mWindow);
    sdl->SetMainRenderer(mRenderer);

    sdl->SetMainWindowWidth(pxWidth);
    sdl->SetMainWindowHeight(pxHeight);

    mInitialized = true;
}


void Game::
InitSdlService()
{
    if(nullptr == Services::GetSDLControl()){
        Services::SetSDLControl( &mSdlControl );
    }
}


void Game::
DestroyContext()
{
    if(mInitialized){
        SDLControl* sdl = Services::GetSDLControl();

        if( &mSdlControl == Services::GetSDLControl() ){
            sdl->SetMainRenderer(nullptr);
            sdl->SetMainWindow(nullptr);
            sdl->DestroyRenderer(mRenderer);
            sdl->DestroyWindow(mWindow);
            Services::SetSDLControl(nullptr);
        }
        mRenderer = nullptr;
        mWindow = nullptr;
    }
    mInitialized = false;
}
