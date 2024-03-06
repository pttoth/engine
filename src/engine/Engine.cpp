#include "engine/Engine.h"

#include "engine/gl/GlWrapper.h"
#include "engine/BillboardComponent.h"
#include "engine/CameraPerspective.h"
#include "engine/Component.h"
#include "engine/DrawingControl.h"
#include "engine/EngineEvent.h"
#include "engine/Scheduler.h"
#include "engine/SDLManager.h"
#include "engine/Services.h"
#include "SDLWrapper.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include "SDL2/SDL.h"
#include <assert.h>
#include <iostream>

using namespace pt;
using namespace engine;

pt::Config      engine::Engine::stCfg                   = pt::Config();
std::string     engine::Engine::stCfgPath               = std::string();
bool            engine::Engine::stInitialized           = false;
Uint32          engine::Engine::stInitializationTime    = 0;
Uint32          engine::Engine::stLastTickTime          = 0;
SDL_Window*     engine::Engine::stMainSDLWindow         = nullptr;
SDL_GLContext   engine::Engine::stMainGLContext         = nullptr;
Uint32          engine::Engine::stUserEventType         = 0;

//--------------------------------------------------
//  temporarily hardcoded shaders
//--------------------------------------------------

const char* DefaultVertexShader = R"(
    #version 330
    precision highp float;

    uniform mat4        M;
    uniform mat4        V;
    uniform mat4        Vrot;
    uniform mat4        PV;
    uniform mat4        PVM;

    layout(location = 0) in vec3 in_vPos;
    layout(location = 1) in vec2 in_tPos;
    layout(location = 2) in vec3 in_Normal;

    out     vec3    vPos;
    out     vec2    tPos;
    out     vec3    vNormal;

    void main(){
        gl_Position = PVM * vec4(in_vPos, 1.0f);
        tPos    = in_tPos;
        vNormal = in_Normal;
        //vNormal = (vec4(in_Normal, 0.0f) * M).xyz;
    }
)";


const char* DefaultGeometryShader = R"()";
const char* DefaultFragmentShader = R"(
    #version 330
    precision highp float;

    uniform sampler2D   gSampler;

    uniform int     AmbientLight_UseAlphaOverride;
    uniform float   AmbientLight_Alpha;

    uniform vec3    LightAmbient;

    in      vec3    vPos;
    in      vec2    tPos;
    in      vec3    vNormal;
    out     vec4    FragColor;

//TODO: delet dis
//    float GetAlpha(){
//        if( 0 == AmbientLight_UseAlphaOverride ){
//            return 1.0f;
//        }else{
//            return AmbientLight_Alpha;
//        }
//    }

    void main(){
        //float a = GetAlpha();

        //vec4 texel = vec4(1.0f,1.0f,1.0f,1.0f);
        vec4 texel = texture( gSampler, tPos );

        FragColor = vec4( texel.xyz * LightAmbient, texel.w);
    }
)";



//--------------------------------------------------
//  TimerData
//--------------------------------------------------

std::vector<Uint64> vec;


//--------------------------------------------------
//  Engine
//--------------------------------------------------


Uint32 Engine::
CreateUserEventType()
{
    if( 0 == stUserEventType ){
        stUserEventType = sdl::RegisterEvents( 1 );

        if( (stUserEventType == 0)
          || (stUserEventType == (Uint32) -1) )  // 0xFFFFFFFF = (Uint32) -1
        {
            PT_LOG_ERR( "Could not register custom UserEvent in SDL! Exiting..." );
            exit(1);
        }
    }
    return stUserEventType;
}

Uint32 Engine::
GetUserEventType()
{
    return stUserEventType;
}


Engine::
Engine():
    SDLApplication()
{
    Construct();
}


Engine::
Engine(int const argc, char* argv[]):
    SDLApplication(argc, argv)
{
    Construct();
}


Engine::
~Engine()
{}


bool Engine::
DeveloperMode() const
{
    return mDeveloperMode;
}


void Engine::
DeveloperMode( bool value )
{
    mDeveloperMode = value;
}


void Engine::
Execute()
{
    assert( !mMainLoopActive );
    if( mMainLoopActive ){
        return;
    }

    OnStart();
    mMainLoopActive = true;

    while( mMainLoopActive ){
        Update();
    }
    OnExit();
}


bool Engine::
Initialize()
{
    if( !stInitialized ){
        PT_LOG_OUT( "-----------------------------------------" );
        PT_LOG_OUT( "Initializing engine" );
        PT_LOG_OUT( "-----" );

        stInitialized = false;

        if( !InitializePtlib() ){
            return false;
        }

        if( !InitializeSDL_GL() ){
            return false;
        }

        if( !InitializeServices() ){
            return false;
        }

        if( !InitializeActorAndComponentData() ){
            return false;
        }

        PT_LOG_OUT( "Engine initialized" );
        PT_LOG_OUT( "-----------------------------------------" );
        stInitialized = true;
    }

    return stInitialized;
}


void Engine::
OnStart()
{
    SDLApplication::OnStart();

    auto sysmgr = NewPtr<SystemManager>();
    Services::SetSystemControl( sysmgr );

    PT_LOG_OUT( "----- software, system and hardware -----" );
    PT_LOG_OUT( "LibPNG:    " << sysmgr->GetLibPNGInfo() );
    PT_LOG_OUT( "Zlib:      " << sysmgr->GetZLibInfo() );
    PT_LOG_OUT( "LibAssimp: " << sysmgr->GetAssimpInfo() );
    //PT_LOG_OUT( "-----" );
    //PT_LOG_OUT( sysmgr->GetCPUInfo() );
    //PT_LOG_OUT( "-----" );
    //PT_LOG_OUT( sysmgr->GetGPUInfo() );
    PT_LOG_OUT( "-----" );
    PT_LOG_OUT( sysmgr->GetGraphicsAPIInfo() );
    PT_LOG_OUT( "-----------------------------------------" );

    Services::SetEngineControl( this );

    auto sdlc = NewPtr<SDLManager>();
    Services::SetSDLControl( sdlc );

    mDrawingManager = NewPtr<DrawingManager>();
    Services::SetDrawingControl( mDrawingManager );

    mWorld = NewPtr<World>();
    Services::SetWorld( mWorld );

    mScheduler = NewPtr<SerialScheduler>();
    Services::SetScheduler( mScheduler );

    sdlc->SetMainWindow( stMainSDLWindow );
    SDL_ShowWindow( stMainSDLWindow );

    // load main vertex and fragment shader source code
    gl::ConstStdSharedPtr vertexShaderSource    = NewPtr<const std::string>( DefaultVertexShader );
    gl::ConstStdSharedPtr fragmentShaderSource  = NewPtr<const std::string>( DefaultFragmentShader );
    //set up shaders
    static const pt::Name vertexShaderName( "MainVertexShader" );
    static const pt::Name fragmentShaderName( "MainFragmentShader" );
    static const pt::Name shaderProgramName( "MainShaderProgram" );
    mVertexShader   = NewPtr<gl::Shader>( vertexShaderName, gl::ShaderType::VERTEX_SHADER, vertexShaderSource );
    mFragmentShader = NewPtr<gl::Shader>( fragmentShaderName, gl::ShaderType::FRAGMENT_SHADER, fragmentShaderSource );
    mShaderProgram  = NewPtr<gl::ShaderProgram>( shaderProgramName );
    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mFragmentShader );
    mShaderProgram->Link();
    mShaderProgram->Use();
    //TODO: initialize shader variables
    static const pt::Name nameM( "M" );
    static const pt::Name nameV( "V" );
    static const pt::Name nameVrot( "Vrot" );
    static const pt::Name namePV( "PV" );
    static const pt::Name namePVM( "PVM" );
    gl::Uniform<math::float4x4> uniM    = mShaderProgram->GetUniform<math::float4x4>( nameM );
    gl::Uniform<math::float4x4> uniV    = mShaderProgram->GetUniform<math::float4x4>( nameV );
    gl::Uniform<math::float4x4> uniVrot = mShaderProgram->GetUniform<math::float4x4>( nameVrot );
    gl::Uniform<math::float4x4> uniPV   = mShaderProgram->GetUniform<math::float4x4>( namePV );
    gl::Uniform<math::float4x4> uniPVM  = mShaderProgram->GetUniform<math::float4x4>( namePVM );

    //assert( /* this should not compile! */ false );
    //gl::Uniform<math::float4x4*> asdasd = mShaderProgram->GetUniform<math::float4x4*>( namePVM );

    mDrawingManager->SetDefaultShaderProgram( mShaderProgram );



    mCamera = NewPtr<CameraPerspective>( "MainCamera" );
    Services::GetDrawingControl()->SetMainCamera( mCamera );


    //TODO: review...
    //configure variables
    bool successful_read = ReadConfig();
    if( !successful_read ){
        PT_LOG_ERR( "Could not read config file: '" << stCfgPath << "'" );
        SetDefaultSettings();
    }
}


void Engine::
OnExit()
{
    //TODO: implement...
    PT_LOG_DEBUG( "Engine::OnExit() is unimplemented!" );
    SDLApplication::OnExit();
}


void Engine::
OnShutdownSignal()
{
    SignalShutdownReady();
}


void Engine::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseMotion(int32_t x, int32_t y,
              int32_t x_rel, int32_t y_rel,
              uint32_t timestamp, uint32_t mouseid)
{}


void Engine::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void Engine::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void Engine::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void Engine::
OnTouchInputEvent()
{
    assert( false );
}


void Engine::
Construct()
{

}


bool Engine::
InitializeActorAndComponentData()
{
    bool result = true;
    result &= engine::BillboardComponent::Initialize();
    return result;
}


bool Engine::
InitializePtlib()
{
    PT_LOG_DEBUG( "Initializing 'ptlib'" );
    bool result = true;
    // Config
    stCfgPath = std::string("../../cfg/Engine.cfg");
    stCfg.setPath( stCfgPath );
    PT_LOG_OUT( "Loaded config: '" << stCfgPath << "'" );

    // Logging
//    result &= pt::log::Initialize( "./", pt::log::AutoGenerateLogFileName() );

    return result;
}


bool Engine::
InitializeSDL_GL()
{
    PT_LOG_DEBUG( "Initializing SDL..." );
    // Init SDL
    int init = SDL_Init( SDL_INIT_EVENTS
                         | SDL_INIT_TIMER
                         | SDL_INIT_AUDIO
                         //| SDL_INIT_VIDEO
                         //| SDL_INIT_HAPTIC
                         | SDL_INIT_JOYSTICK
                         | SDL_INIT_GAMECONTROLLER
                         );
    if( 0 != init  ){
        PT_LOG_ERR( "Failed to initialize SDL." );
        return false;
    }
    PT_LOG_DEBUG( "  SUCCESS - Subsystems" );
    stInitializationTime = SDL_GetTicks();
    atexit( SDL_Quit );
    CreateUserEventType();
    PT_LOG_DEBUG( "  SUCCESS - Custom UserEvent type" );


    // create hidden SDL Window (needed in order to have an OpenGL context)
    stMainSDLWindow = SDL_CreateWindow( "Indicus Engine Main Window",
                                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        1280, 720,
                                        SDL_WINDOW_HIDDEN
                                        | SDL_WINDOW_OPENGL
                                        );
    if( 0 == stMainSDLWindow ){
        PT_LOG_ERR( "Failed to create main SDL window."
                    "\n  reason: " << SDL_GetError() );
        return false;
    }
    auto windowGuard = pt::CreateGuard( []{
        SDL_DestroyWindow( stMainSDLWindow );
    } );
    PT_LOG_DEBUG( "  SUCCESS - Main SDL Window" );

    //-----
    // Init OpenGL
    stMainGLContext = sdl::GL_CreateContext( stMainSDLWindow );
    if( nullptr == stMainGLContext ){
        PT_LOG_ERR( "Failed to create OpenGL context"
                    "\n  reason: " << SDL_GetError() );
        return false;
    }
    auto glContextGuard = pt::CreateGuard( []{
        SDL_GL_DeleteContext( stMainGLContext );
    } );
    PT_LOG_DEBUG( "  SUCCESS - OpenGL context" );


    //init glew
    glewExperimental = GL_TRUE;     //note: must be before glewInit() !
    GLenum res = glewInit();
    if ( res != GLEW_OK ){
        PT_LOG_ERR( "Failed to initialize GLEW"
                    << "\n  error: '" << glewGetErrorString(res) << "'" );
        return false;
    }
    PT_LOG_DEBUG( "  SUCCESS - GLEW" );

    glContextGuard.Disable();
    windowGuard.Disable();
    return true;
}


bool Engine::
InitializeServices()
{
    Services::Instance(); // create Service provider

    return true;
}


void Engine::
SetDefaultSettings()
{}


bool Engine::
ReadConfig()
{
    try{
        //mCfg.read();

    }catch(...){
        return false;
    }
    return true;
}


void Engine::
Update()
{
    SDL_Event ev;
    // handle new events/inputs/etc.
    while( SDL_PollEvent( &ev ) ){
        OnEvent( &ev );
    }

    Uint32 currentTime = SDL_GetTicks() - stInitializationTime;
    bool comp = ( stInitializationTime < stLastTickTime );
    Uint32 lastTickTime = comp * stLastTickTime + comp * stInitializationTime;
    float ft = currentTime / 1000.0f;
    float fdt = (currentTime - lastTickTime) / 1000.0f;
    stLastTickTime = currentTime;

    // Actor Tick+TickDependency [un]registrations get executed here
    mScheduler->ProcessPendingTasks();

    // Tick all Actors
    mScheduler->TickPrePhysics( ft, fdt );
    mScheduler->TickDuringPhysics( ft, fdt );
    mScheduler->TickPostPhysics( ft, fdt );

    //TODO: rename to OnPostGameStateUpdate()
    UpdateGameState( ft, fdt );

    drawScene( ft, fdt );
}


void Engine::
OnEvent(SDL_Event* event)
{
    SDL_Event ev = *event; //avoid unneccessary repeat of dereferences
    switch( ev.type ){
    case SDL_MOUSEMOTION:
        if(! (ev.motion.which & SDL_TOUCH_MOUSEID) ){ //if not touchpad event
            OnMouseMotion( ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel, ev.motion.timestamp, ev.motion.which );
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        OnMouseButtonDown( ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which );
        break;
    case SDL_MOUSEBUTTONUP:
        OnMouseButtonUp( ev.button.x, ev.button.y, ev.button.button,ev.button.clicks, ev.button.timestamp, ev.button.which );
        break;
    case SDL_MOUSEWHEEL:
        OnMouseWheel( ev.wheel.x, ev.wheel.y, ev.wheel.timestamp, ev.wheel.which, ev.wheel.direction );
        break;
    case SDL_KEYDOWN:
        OnKeyDown( ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat );
        break;
    case SDL_KEYUP:
        OnKeyUp( ev.key.keysym.sym, ev.key.keysym.mod, ev.key.timestamp, ev.key.repeat );
        break;
    case SDL_FINGERMOTION:
        //TODO: handle...
        break;
    case SDL_FINGERDOWN:
        //TODO: handle...
        break;
    case SDL_FINGERUP:
        //TODO: handle...
        break;
    case SDL_USEREVENT:
        switch( ev.user.code ){
        default:
            break;
        }
        break;
    case SDL_QUIT:
        mMainLoopActive = false;
        //Quit();
        break;
    }
}


//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------


void Engine::
drawScene( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    if( dc != nullptr ){
        dc->DrawScene( t, dt );
    }
}
