#include "engine/test/ogltester01/Game.h"

#include "engine/Services.h"

#include "pt/logging.h"

#include <algorithm>
#include <sstream>


using namespace test;
using namespace test::ogltester01;

using namespace engine;
using namespace pt::math;


Game::
Game()
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
    switch(keycode){
    }
}


void Game::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{
    switch(keycode){
    }
}

void Game::
InitContext()
{
    InitSdlService();

    SDLControl* sdl = Services::GetSDLControl();

    pt::log::out<< "initializing..." << "\n";

    //init SDL
    int init = SDL_Init(SDL_INIT_EVENTS
                        | SDL_INIT_TIMER
                        );
    if( 0 != init){
        throw std::runtime_error("Failed to initialize SDL");
    }

    //set up OpenGL context
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    /*
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    */

    //initialize window
    uint32_t pxWidth = 1280;
    uint32_t pxHeight = 720;

    mWindow = sdl->CreateWindow("OpenGL Tutorials",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                pxWidth, pxHeight,
                                SDL_WINDOW_OPENGL
                                //|SDL_WINDOW_RESIZABLE
                                //|SDL_WINDOW_INPUT_GRABBED
                                );

    if( nullptr == mWindow ){
        std::stringstream ss;
        ss << "Could not create SDL Window! Reason: " << SDL_GetError();
        pt::err << ss.str() << "\n";
        throw std::system_error( ss.str() );
    }

    //create OpenGL context
    *mGlContext = SDL_GL_CreateContext( *mWindow );

    //init glew
    assert(false); //TODO: remove this after integrating OpenGL into project


    glewExperimental = GL_TRUE;     //note: must be before glewInit() !!!
    GLenum res = glewInit();
    if (res != GLEW_OK){
        std::stringstream ss;
        ss << "Could not initialize GLEW! Reason: " << glewGetErrorString(res);

        SDL_DestroyWindow( *window );

        pt::err << ss.str() << "\n";
        throw std::system_error( ss.str() );
    }

    pt::log::out << "-----\n";
    int majorVersion, minorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

    pt::log::out << "GL Vendor    : " << glGetString(GL_VENDOR) << "\n";
    pt::log::out << "GL Renderer  : " << glGetString(GL_RENDERER) << "\n";
    pt::log::out << "GL Version (string)  : " << glGetString(GL_VERSION) << "\n";
    pt::log::out << "GL Version (integer) : " << majorVersion << "." << minorVersion << "\n";
    pt::log::out << "GLSL Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    pt::log::out << "-----\ncompiling shaders...";

    assert(false);
    int failed = true;
    //int failed = CompileShaders();
    if( failed ){
        SDL_DestroyWindow(*window);
        return 1;
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);


    //enable backface culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //draw something
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow( *mWindow );
    pt::log::out << " done\n";


    assert(false); //TODO: move the GL calls into a separate handler class


    assert(false); //TODO: fix the code order below (may need to move up)

    sdl->SetMainWindow(mWindow);


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
            sdl->SetMainWindow(nullptr);
            sdl->DestroyWindow(mWindow);
            Services::SetSDLControl(nullptr);
        }
        mWindow = nullptr;
    }
    mInitialized = false;
}
