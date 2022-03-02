#include "test/ticktester_game.h"

using namespace engine;

TickTesterGame::
        TickTesterGame(){
}

TickTesterGame::
        ~TickTesterGame(){
}

void TickTesterGame::
        onStart(){
    Engine::onStart();
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
    eDepTesterCompSub1.display_tick = true;
    eDepTesterCompSub2.display_tick = true;
    eDepTesterCompDep1.display_tick = true;
    eDepTesterCompDep2.display_tick = true;

    eDependencyTester_dep.setTickInterval(500.0f);
    eDependencyTester_sub.setTickInterval(500.0f);

    eDependencyTester_dep.display_tick = true;
    eDependencyTester_sub.display_tick = true;

    //add dependency
    Entity::AddTickDependency(&eDependencyTester_sub, &eDependencyTester_dep);


//------------
}

void TickTesterGame::
        onExit(){
//------------
//code here...


//------------
    Engine::onExit();
}

void TickTesterGame::
        onShutdownSignal(){
//------------
//code here...


//------------
    Engine::onShutdownSignal();
}

void TickTesterGame::
        tick(float t, float dt){
    //Game::tick(t, dt);

}

void TickTesterGame::
        onMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void TickTesterGame::
        onMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid){
}

void TickTesterGame::
        onMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid){
}

void TickTesterGame::
        onMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction){
}

void TickTesterGame::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
}

void TickTesterGame::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){
}

