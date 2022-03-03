#include "test/ticktester/game.h"

#include "services.h"

using namespace engine;

TickTesterGame::
TickTesterGame():
    mWindow(nullptr), mRenderer(nullptr), mInitialized(false)
{}


TickTesterGame::
~TickTesterGame()
{
    DestroyContext();
}


void TickTesterGame::
onStart()
{
    Engine::onStart();

    InitContext();



}


void TickTesterGame::
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


void TickTesterGame::
onShutdownSignal()
{
    //------------
    //code here...


    //------------
    Engine::onShutdownSignal();
}


void TickTesterGame::
tick(float t, float dt)
{
    //Engine::tick(t, dt);

    SDLControl* sdl = Services::getSDLControl();
    auto renderer = sdl->GetMainRenderer();

    sdl->RenderClear( renderer );
    sdl->RenderPresent( renderer );
}


void TickTesterGame::
onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void TickTesterGame::
onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void TickTesterGame::
onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{}


void TickTesterGame::
onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void TickTesterGame::
onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void TickTesterGame::
onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}

void TickTesterGame::
InitContext()
{
    InitSdlService();

    SDLControl* sdl = Services::getSDLControl();

    mWindow = sdl->CreateWindow("Tick logic test",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                1280, 720, NULL);
    mRenderer = sdl->CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

    sdl->SetMainWindow(mWindow);
    sdl->SetMainRenderer(mRenderer);

    mInitialized = true;
}


void TickTesterGame::
InitSdlService()
{
    if(nullptr == Services::getSDLControl()){
        Services::setSDLControl( &mSdlControl );
    }
}


void TickTesterGame::
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
