#include "test/ticktester/Game.h"

#include "Services.h"

#include "pt/logging.h"

#include <sstream>

using namespace test;
using namespace test::ticktester;

using namespace engine;

Game::
Game():
    mInitialized(false),
    mWindow(nullptr), mRenderer(nullptr),
    mCamera("MainCamera"),
    mPlayerPawn("PlayerPawn")
{
    std::stringstream ss;
    ss << pt::log::AutoGenerateLogFileName();

    pt::log::Initialize("./", ss.str());
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

    //initialize entities
    Entity::RegisterEntity(&mPlayerPawn);
    Entity::RegisterTickFunction(&mPlayerPawn, TickGroup::PREPHYSICS); //TODO: move this to PlayerPawn


    mPlayerPawn.Spawn();
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
Tick(float t, float dt)
{
    //Engine::tick(t, dt);

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


void Game::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    using namespace pt::math;

    auto rootComp = mPlayerPawn.getRootComponent();
    auto bbc = mPlayerPawn.getBBC();
    float3 pos = rootComp->getPosition();
    float3 pos_bbc = bbc->getPosition();

    switch(keycode){
    case SDLK_w:
        rootComp->setPosition(pos + float3(0,0.1f,0));
        break;
    case SDLK_s:
        rootComp->setPosition(pos + float3(0,-0.1f,0));
        break;
    case SDLK_a:
        rootComp->setPosition(pos + float3(-0.1f,0,0));
        break;
    case SDLK_d:
        rootComp->setPosition(pos + float3(0.1f,0,0));
        break;
    case SDLK_u:
        bbc->setPosition(pos_bbc + float3(0,0.05f,0));
        break;
    case SDLK_j:
        bbc->setPosition(pos_bbc + float3(0,-0.05f,0));
        break;
    case SDLK_h:
        bbc->setPosition(pos_bbc + float3(-0.05f,0,0));
        break;
    case SDLK_k:
        bbc->setPosition(pos_bbc + float3(0.05f,0,0));
        break;
    }
}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}

void Game::
InitContext()
{
    InitSdlService();

    SDLControl* sdl = Services::GetSDLControl();


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
