#include "engine/test/ActorTester/Game.h"

#include "engine/Services.h"
#include "engine/SDLControl.h"
#include "engine/DrawingControl.h"
#include "GlWrapper.h"
#include "SDLWrapper.h"
#include "ShaderProgram.h"

#include "pt/logging.h"

#include "GL/glew.h"
#include "GL/gl.h"


#include <sstream>

using namespace test;
using namespace test::actortester;

using namespace pt;
using namespace pt::math;
using namespace engine;

const char* test::actortester::Game::VertexShader = R"(
    #version 330
    precision highp float;

    struct Sphere{
        bool  isEnabled;
        float r;
    };

    uniform Sphere      shapes_sphere[64];

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


const char* test::actortester::Game::GeometryShader = R"()";
const char* test::actortester::Game::FragmentShader = R"(
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
    out     vec4    FragColor;    auto dc = Services::GetDrawingControl();
    dc->DrawScene(t, dt);

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
Game():
    engine::Engine(),
    mCamera( "mCamera" ),
    mPawn( "mPawn" )
{
    std::stringstream ss;
    ss << pt::log::AutoGenerateLogFileName();

    pt::log::Initialize("./", ss.str());
}


Game::
~Game()
{
    //TODO:
    /*
    if(mInitialized){
        DestroyContext();
    }
    */
}


void Game::
OnStart()
{
    Engine::OnStart();

    Services::SetScheduler( &mScheduler );

    InitContext();

    Services::SetDrawingControl( &mDrawingManager );
    mDrawingManager.SetMainCamera( &mCamera );

    //initialize entities
    Actor::RegisterTickFunction( mPawn, TickGroup::PREPHYSICS );

    mPawn.Spawn();
}


void Game::
OnExit()
{
    //------------
    //code here...

    mPawn.Despawn();

    Services::GetDrawingControl()->SetMainCamera( nullptr );

    if( mInitialized ){
        DestroyContext();
    }
    //------------
    Engine::OnExit();
}


void Game::
UpdateGameState( float t, float dt )
{
    //Engine::Update(t, dt);

    float mainSpeed = 2.0f;
    float subSpeed = 2.0f;

    {
        bool updateMain = mButtonPressedMoveMainUp
                          || mButtonPressedMoveMainDown
                          || mButtonPressedMoveMainLeft
                          || mButtonPressedMoveMainRight;
        float3 updateMainVec;

        if( mButtonPressedMoveMainUp ){
            updateMainVec.y += mainSpeed * dt;
        }
        if( mButtonPressedMoveMainDown ){
            updateMainVec.y -= mainSpeed * dt;
        }
        if( mButtonPressedMoveMainLeft ){
            updateMainVec.x -= mainSpeed * dt;
        }
        if( mButtonPressedMoveMainRight ){
            updateMainVec.x += mainSpeed * dt;
        }
        if( updateMain ){
            float3 pos = mPawn.GetPosition();
            mPawn.SetPosition( pos + updateMainVec );
        }
    }
}


void Game::
OnMouseButtonDown( int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseButtonDown( x, y, button, clicks, timestamp, mouseid );

}


void Game::
OnMouseButtonUp( int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseButtonUp( x, y, button, clicks, timestamp, mouseid );

}


void Game::
OnMouseMotion( int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid )
{
    Engine::OnMouseMotion( x, y, x_rel, y_rel, timestamp, mouseid );

}


void Game::
OnMouseWheel( int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction )
{
    Engine::OnMouseWheel( x, y, timestamp, mouseid, direction );

}


void Game::
OnKeyDown( SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat )
{
    Engine::OnKeyDown( keycode, keymod, timestamp, repeat );

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
/*
    case SDLK_q:
        mButtonPressedIncreaseRadius = true;
        break;
    case SDLK_e:
        mButtonPressedDecreaseRadius = true;
        break;
    case SDLK_KP_PLUS:
        mTickrateTableIdx = pt::Clamp(mTickrateTableIdx+1, (size_t) 0, mTickrateTable.size() -1) ;
        this->SetTickrate( mTickrateTable[mTickrateTableIdx] );
        break;
    case SDLK_KP_MINUS:
        ++mTickrateTableIdx; //to avoid unsigned underflow
        mTickrateTableIdx = pt::Clamp(mTickrateTableIdx-1, (size_t) 1, mTickrateTable.size()) -1;
        this->SetTickrate( mTickrateTable[mTickrateTableIdx] );
        break;
*/
    }
}


void Game::
OnKeyUp( SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat )
{
    Engine::OnKeyUp( keycode, keymod, timestamp, repeat );

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
    /*
    case SDLK_q:
        mButtonPressedIncreaseRadius = false;
        break;
    case SDLK_e:
        mButtonPressedDecreaseRadius = false;
        break;
    */
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
        throw std::runtime_error( "Failed to initialize SDL" );
    }

    //initialize window
    uint32_t pxWidth = 1280;
    uint32_t pxHeight = 720;

    //pxWidth = 960;
    //pxHeight = 960;

    uint32_t flags = 0;

    mWindow = sdl::CreateWindow( "Actor tick logic test",
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 pxWidth, pxHeight, flags );
    mRenderer = sdl::CreateRenderer( mWindow, -1, SDL_RENDERER_ACCELERATED );

    if( nullptr == mWindow ){
        std::stringstream ss;
        ss << "Could not create SDL Window! Reason: " << SDL_GetError();
        pt::err << ss.str() << "\n";
        throw std::runtime_error( ss.str() );
    }

    sdl->SetMainWindow( mWindow );
    sdl->SetMainRenderer( mRenderer );

    sdl->SetMainWindowWidth( pxWidth );
    sdl->SetMainWindowHeight( pxHeight );

    mInitialized = true;
}


void Game::
InitSdlService()
{
    if( nullptr == Services::GetSDLControl() ){
        Services::SetSDLControl( &mSdlControl );
    }
}


void Game::
Execute()
{
    Engine::Engine::Execute();
    SDL_Delay(16);  // hardcoded ~60 fps game loop
}


void Game::
DestroyContext()
{
    if( mInitialized ){
        SDLControl* sdl = Services::GetSDLControl();

        if( &mSdlControl == sdl ){
            sdl->SetMainRenderer( nullptr );
            sdl->SetMainWindow( nullptr );
            sdl::DestroyRenderer( mRenderer );
            sdl::DestroyWindow( mWindow );
            Services::SetSDLControl( nullptr );
        }
        mRenderer = nullptr;
        mWindow = nullptr;
    }
    mInitialized = false;
}
