#include "testdrawopengl.h"

using namespace pttoth;
using namespace pttoth::engine;

TestDrawOpenGL::
        TestDrawOpenGL(){

}

void TestDrawOpenGL::
        onStart(){
    Game::onStart();
    //...
    window = SDL_CreateWindow("OpenGL test",
                              64, 64,
                              800, 600,
                              SDL_WINDOW_OPENGL);
    drawer.init(window);

}

void TestDrawOpenGL::
        onExit(){
    //...

    //-------------
    Game::onExit();
}

void TestDrawOpenGL::
        tick(float t, float dt){
    Game::tick(t,dt);
    drawer.render(t, dt);
}

void TestDrawOpenGL::
        onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){

}

void TestDrawOpenGL::
        onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat){

}
