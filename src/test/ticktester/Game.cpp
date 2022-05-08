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
    DestroyContext();
}


void Game::
onStart()
{
    Engine::onStart();

    InitContext();

    Services::getDrawingControl()->SetMainCamera(&mCamera);

    //initialize entities
    Entity::RegisterEntity(&mPlayerPawn);
    Entity::RegisterTickFunction(&mPlayerPawn, TickGroup::PREPHYSICS); //TODO: move this to PlayerPawn


    mPlayerPawn.Spawn();
}


void Game::
onExit()
{
    //------------
    //code here...

    if(mInitialized){
        DestroyContext();
    }
    //------------
    Engine::onExit();
}


void Game::
onShutdownSignal()
{
    //------------
    //code here...


    //------------
    Engine::onShutdownSignal();
}


void Game::
tick(float t, float dt)
{
    //Engine::tick(t, dt);

    auto dc = Services::getDrawingControl();
    dc->DrawScene(t, dt);
}


void Game::
onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{}


void Game::
onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void Game::
onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    using namespace pt::math;

    auto rootComp = mPlayerPawn.getRootComponent();
    float3 pos = rootComp->getPosition();

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
    }
}


void Game::
onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}

void Game::
InitContext()
{
    InitSdlService();

    SDLControl* sdl = Services::getSDLControl();


    //initialize window
    uint32_t pxWidth = 1280;
    uint32_t pxHeight = 720;
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
    if(nullptr == Services::getSDLControl()){
        Services::setSDLControl( &mSdlControl );
    }
}


void Game::
DestroyContext()
{
    SDLControl* sdl = Services::getSDLControl();

    if( &mSdlControl == Services::getSDLControl() ){
        sdl->SetMainRenderer(nullptr);
        sdl->SetMainWindow(nullptr);
        sdl->DestroyRenderer(mRenderer);
        sdl->DestroyWindow(mWindow);
        Services::setSDLControl(nullptr);
    }
    mRenderer = nullptr;
    mWindow = nullptr;
}
