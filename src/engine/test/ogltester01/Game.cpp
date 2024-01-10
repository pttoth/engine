#include "engine/test/ogltester01/Game.h"

#include "engine/Services.h"

#include "GL/glew.h"
#include "GL/gl.h"


#include "pt/logging.h"
#include "GlWrapper.h"
#include "SDLWrapper.h"
#include "ShaderProgram.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <string>


using namespace test;
using namespace test::ogltester01;

using namespace engine;
using namespace pt;
using namespace math;


const char* VertexShader = R"(
    #version 330
    precision highp float;

    uniform float       Scale;
    uniform mat4        M;
    uniform mat4        VP;
    uniform mat4        MVP;
    uniform int         UseFixColor;
    uniform vec3        Color;

    uniform vec3        LightAmbient;

    layout(location = 0) in vec3 in_vPos;
    layout(location = 1) in vec2 in_tPos;
    layout(location = 2) in vec3 in_Normal;

    out     vec3    vPos;
    out     vec2    tPos;
    out     vec3    Normal;

    void main(){
        gl_Position = vec4(in_vPos, 1.0f) * MVP;
        if(0 == UseFixColor){
            tPos    = in_tPos;
        }else{
            tPos    = in_tPos; //don't care
        }
        //Normal = (vec4(in_Normal, 0.0f) * M).xyz;
        Normal = in_Normal;
    }
)";


const char* GeometryShader = R"()";
const char* FragmentShader = R"(
    #version 330
    precision highp float;

    struct Spotlight{
        bool    enabled;
        vec3    color;
        float   intensity_diffuse;
        vec3    direction;
    };

    uniform sampler2D       gSampler;
    uniform int             UseFixColor;
    uniform vec3            Color;
    uniform int             UseAlphaOverride;
    uniform float           Alpha;

    uniform vec3            LightAmbient;
    uniform Spotlight       Spotlights[64];


    in      vec3    vPos;
    in      vec2    tPos;
    out     vec3    Normal;
    out     vec4    FragColor;

    float GetAlpha(){
        if(0 == UseAlphaOverride){  return 1.0f;
        }else{                      return Alpha;
        }
    }

    void main(){
        float a = GetAlpha();
        vec4 retval = vec4(1.0f,1.0f,1.0f,1.0f);

        for(int i=0; i<64;++i){
            if(Spotlights[i].enabled){
                //calculate spotlight effects
            }
        }

        if(0 == UseFixColor){   retval = texture(gSampler, tPos);
        }else{                  retval = vec4(Color, a);
        }

        FragColor = vec4( retval.xyz * LightAmbient, retval.w);
    }
)";

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
Execute()
{
    Engine::Engine::Execute();
    SDL_Delay(16);  // hardcoded ~60 fps game loop
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
UpdateGameState(float t, float dt)
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

    mWindow = sdl::CreateWindow("OpenGL Tutorials",
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
        throw std::runtime_error( ss.str() );
    }

    //create OpenGL context
    mGlContext = SDL_GL_CreateContext( mWindow );

    //init glew
    glewExperimental = GL_TRUE;     //note: must be before glewInit() !!!
    GLenum res = glewInit();
    if (res != GLEW_OK){
        std::stringstream ss;
        ss << "Could not initialize GLEW! Reason: " << glewGetErrorString(res);

        SDL_DestroyWindow( mWindow );

        pt::err << ss.str() << "\n";
        throw std::runtime_error( ss.str() );
    }

    pt::log::out << "-----\n";
    int majorVersion, minorVersion;
    gl::GetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    gl::GetIntegerv(GL_MINOR_VERSION, &minorVersion);

    pt::log::out << "GL Vendor    : " << gl::GetString(GL_VENDOR) << "\n";
    pt::log::out << "GL Renderer  : " << gl::GetString(GL_RENDERER) << "\n";
    pt::log::out << "GL Version (string)  : " << gl::GetString(GL_VERSION) << "\n";
    pt::log::out << "GL Version (integer) : " << majorVersion << "." << minorVersion << "\n";
    pt::log::out << "GLSL Version : " << gl::GetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    pt::log::out << "-----\ncompiling shaders...";

    try{
        ShaderProgram shaderprogram( std::string{VertexShader},
                                     std::string{GeometryShader},
                                     std::string{FragmentShader});
    }catch( const std::exception& e ){
        pt::log::err << "Couldn't create shaderprogram!\n";
        //TODO: free allocated stuff
        throw;
    }

    //enable depth testing
    gl::Enable(GL_DEPTH_TEST);

    //enable backface culling
    gl::FrontFace(GL_CW);
    gl::CullFace(GL_BACK);
    gl::Enable(GL_CULL_FACE);
    gl::Enable (GL_BLEND);
    gl::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //draw something
    gl::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow( mWindow ); //TODO: may need to be moved under a mutex
    pt::log::out << " done\n";


    //assert(false); //TODO: fix the code order below (may need to move up)

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
            sdl::DestroyWindow(mWindow);
            Services::SetSDLControl(nullptr);
        }
        mWindow = nullptr;
    }
    mInitialized = false;
}
