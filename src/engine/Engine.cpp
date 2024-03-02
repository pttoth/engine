#include "engine/Engine.h"

#include "engine/gl/GlWrapper.h"
#include "engine/CameraPerspective.h"
#include "engine/Component.h"
#include "engine/DrawingControl.h"
#include "engine/EngineEvent.h"
#include "engine/Scheduler.h"
#include "engine/SDLControl.h"
#include "engine/Services.h"
#include "SDLWrapper.h"
#include "pt/logging.h"
#include "SDL2/SDL.h"
#include <assert.h>
#include <iostream>

using namespace pt;
using namespace engine;

Uint32 engine::Engine::mUserEventType = 0;

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

        FragColor = vec4( retval.xyz * LightAmbient, texel.w);
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
GetUserEventType()
{
    //TODO: engine mutex here
    if( 0 == mUserEventType ){
        mUserEventType = sdl::RegisterEvents( 1 );

        if( mUserEventType == 0
          || mUserEventType == (Uint32) -1 )     // 0xFFFFFFFF = (Uint32) -1
        {
            pt::log::err << "Could not register custom UserEvent in SDL! Exiting...\n";
            exit(1);
        }
    }
    return mUserEventType;
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


void Engine::
Construct()
{
    mCfgPath= std::string("../../cfg/Engine.cfg");
    GetUserEventType(); // first call generates the UserEvent type code
    InitializeConfig();
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
    assert( not mMainLoopActive );
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


void Engine::
OnStart()
{
    SDLApplication::OnStart();
    int init = SDL_Init( SDL_INIT_EVENTS
                         | SDL_INIT_TIMER
                         | SDL_INIT_AUDIO
                         //| SDL_INIT_VIDEO
                         //| SDL_INIT_HAPTIC
                         | SDL_INIT_JOYSTICK
                         | SDL_INIT_GAMECONTROLLER
                         );
    if( 0 != init  ){
        //TODO: remove this and everything related to this handling logic!
        const std::string errormsg( "Failed to initialize SDL" );
        this->SetErrorMessage( errormsg.c_str() );

        throw std::runtime_error( errormsg.c_str() );
    }
    mUptime = SDL_GetTicks();

    atexit( SDL_Quit );

    // GL context is created here
    mDrawingManager.Initialize();

    // load main vertex and fragment shader source code


    gl::ConstStdSharedPtr emptyShader;


    //set up shaders
    static const pt::Name vertexShaderName( "MainVertexShader" );
    static const pt::Name fragmentShaderName( "MainFragmentShader" );
    static const pt::Name shaderProgramName( "MainShaderProgram" );
    mVertexShader   = NewPtr<gl::Shader>( vertexShaderName, gl::ShaderType::VERTEX_SHADER, emptyShader );
    mFragmentShader = NewPtr<gl::Shader>( fragmentShaderName, gl::ShaderType::FRAGMENT_SHADER, emptyShader );
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

    assert( /* this should not compile! */ false );
    gl::Uniform<math::float4x4*> asdasd = mShaderProgram->GetUniform<math::float4x4*>( namePVM );

    mDrawingManager.SetDefaultShaderProgram( mShaderProgram );
    Services::SetDrawingControl( &mDrawingManager );


    Services::SetScheduler( &mScheduler );

    Services::SetWorld( &mWorld );

    mCamera = NewPtr<CameraPerspective>( "MainCamera" );
    Services::GetDrawingControl()->SetMainCamera( mCamera );


    //configure variables
    bool successful_read = ReadConfig();
    if( !successful_read ){
        std::cout << "warning: could not read config file: "
                  << mCfgPath << std::endl;
        SetDefaultSettings();
    }

}


void Engine::
OnExit()
{
    if( 0 != mGametimerId ){
        SDL_RemoveTimer( mGametimerId );
        mGametimerId = 0;
    }

    const World* const w = Services::GetWorld();
    if( w == &mWorld ){
        Services::SetWorld( nullptr );
    }

    const Scheduler* const sched = Services::GetScheduler();
    if( sched == &mScheduler ){
        Services::SetScheduler( nullptr );
    }

    const DrawingControl* const dc = Services::GetDrawingControl();
    if( dc == &mDrawingManager ){
        Services::SetDrawingControl( nullptr );
    }

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
InitializeConfig()
{
    mCfg.setPath( mCfgPath );
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

    Uint32 current_time = SDL_GetTicks();
    float ft = current_time / 1000.0f;
    float fdt = (current_time - mUptime) / 1000.0f;
    mUptime = current_time;

    // Actor Tick+TickDependency [un]registrations get executed here
    mScheduler.ProcessPendingTasks();

    // Tick all Actors
    mScheduler.TickPrePhysics( ft, fdt );
    mScheduler.TickDuringPhysics( ft, fdt );
    mScheduler.TickPostPhysics( ft, fdt );

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
