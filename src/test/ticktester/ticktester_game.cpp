#include "test/ticktester_game.h"

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


    //------------
    //code here...


    //test entity registration

    //test entity unreg

    //test entity double registration

    //test component registration

    //test component double registration

    //test tick registration
        //test default values
        //expected:
            //should tick
            //during physics
        //test tickgroups
            //prephys
            //postphys
        //test enable/disable tick

    //test tick dependency


/*
    //csekkold reg nélkül
    Entity::RegisterEntity(&emouse);
    Entity::RegisterEntity(&ekeyboard);
    Entity::RegisterTickFunction(&emouse);
    Entity::RegisterTickFunction(&ekeyboard);

    ekeyboard.display_tick = true;
    emouse.display_tick = true;

    ekeyboard.setTickInterval(100.0f);
    emouse.setTickInterval(500.0f);

    emouse.setID("mouse_tester");
    ekeyboard.setID("kboard_tester");

*/


    //dependency check
    eDependencyTester_sub.addComponent(&eDepTesterCompSub1);
    eDependencyTester_sub.addComponent(&eDepTesterCompSub2);
    eDependencyTester_dep.addComponent(&eDepTesterCompDep1);
    eDependencyTester_dep.addComponent(&eDepTesterCompDep2);

    eDependencyTester_sub.setID("subject");
    Entity::RegisterEntity(&eDependencyTester_sub);
    Entity::RegisterTickFunction(&eDependencyTester_sub);

    //reg the dependency last
    eDependencyTester_dep.setID("dependency");
    Entity::RegisterEntity(&eDependencyTester_dep);
    Entity::RegisterTickFunction(&eDependencyTester_dep);

    eDepTesterCompSub1.setID("EntitySub1");
    eDepTesterCompSub2.setID("EntitySub2");
    eDepTesterCompDep1.setID("ComponentDep1");
    eDepTesterCompDep2.setID("ComponentDep2");
    eDepTesterCompSub1.mDisplayTick = true;
    eDepTesterCompSub2.mDisplayTick = true;
    eDepTesterCompDep1.mDisplayTick = true;
    eDepTesterCompDep2.mDisplayTick = true;

    eDependencyTester_dep.setTickInterval(500.0f);
    eDependencyTester_sub.setTickInterval(500.0f);

    eDependencyTester_dep.mDisplayTick = true;
    eDependencyTester_sub.mDisplayTick = true;

    //add dependency
    Entity::AddTickDependency(&eDependencyTester_sub, &eDependencyTester_dep);


    //------------
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
