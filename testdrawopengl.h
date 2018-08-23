#include "game.h"

#include "openglrenderer.h"

namespace pttoth{
namespace engine{


class TestDrawOpenGL: public pttoth::engine::Game{

    pttoth::engine::OpenGLRenderer drawer;

public:
    TestDrawOpenGL();

    // Application interface
protected:
    void onStart();
    void onExit();

    // Game interface
protected:
    void tick(float t, float dt);
    void onKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
    void onKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat);
};


} }
