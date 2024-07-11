#include "engine/Engine.h"

#include "engine/gl/GlWrapper.h"
#include "engine/StandardShaderProgram.h"
#include "engine/service/DrawingControl.h"
#include "engine/service/Scheduler.h"
#include "engine/service/SDLManager.h"
#include "engine/Services.h"
#include "engine/actor/CameraPerspective.h"
#include "engine/component/BillboardComponent.h"
#include "engine/component/Component.h"
#include "SDLWrapper.h"
#include "pt/guard.hpp"
#include "pt/logging.h"
#include "SDL2/SDL.h"
#include <assert.h>
#include <iostream>

using namespace pt;
using namespace engine;
using namespace math;

enum ConfigKeys{
    iDefaultResWidth,
    iDefaultResHeight
};

pt::Config      engine::Engine::stCfg                   = pt::Config();
std::string     engine::Engine::stCfgPath               = std::string();
bool            engine::Engine::stInitialized           = false;
Uint32          engine::Engine::stInitializationTime    = 0;
Uint32          engine::Engine::stLastTickTime          = 0;
SDL_Window*     engine::Engine::stMainSDLWindow         = nullptr;
SDL_GLContext   engine::Engine::stMainGLContext         = nullptr;
Uint32          engine::Engine::stUserEventType         = 0;

int32_t         engine::Engine::stDefaultResWidth       = 640;
int32_t         engine::Engine::stDefaultResHeight      = 360;


const pt::Name engine::Engine::nameVertexShader( "MainVertexShader" );
const pt::Name engine::Engine::nameFragmentShader( "MainFragmentShader" );
const pt::Name engine::Engine::nameShaderProgram( "MainShaderProgram" );

const pt::Name engine::Engine::nameT( "t" );
const pt::Name engine::Engine::nameDT( "dt" );
const pt::Name engine::Engine::nameM( "M" );
const pt::Name engine::Engine::nameV( "V" );
const pt::Name engine::Engine::nameVrot( "Vrot" );
const pt::Name engine::Engine::namePV( "PV" );
const pt::Name engine::Engine::namePVM( "PVM" );

//--------------------------------------------------
//  temporarily hardcoded shaders
//--------------------------------------------------


const char* DefaultVertexShader = R"(
    #version 330
    precision highp float;

    // ----- shader/FrameInfo.ubo -----
    // FrameInfo v0.1
    layout(std140) uniform FrameInfo{
        // Row and column-major notations and clarifications in the GL specs are awful, they only serve to confuse.
        // C memory layout is row-oriented, but the value layout in it is column-major!
        // Don't get deceived by the 'layout(row_major)' definitions for matrices.
        // Both in C structs and the shader code, all matrices are column-major !!! (P * V * M) !!!
                          float t;          // current time
                          float dt;         // delta time since last frame
        layout(row_major) mat4  V;          // View matrix
        layout(row_major) mat4  Vrot;       // View matrix rotation component (without translation)
        layout(row_major) mat4  P;          // Projection matrix
        layout(row_major) mat4  PV;         // P * V
        layout(row_major) mat4  PVrotInv;   // inv(Vrot) * inv(P)
    } frameInfo;
    // --------------------------------



    uniform int         AxisDrawMode;
    uniform int         SkyboxMode;
    uniform mat4        M;
    uniform mat4        PVM;

    layout(location = 0) in vec3 in_vPos;
    layout(location = 1) in vec2 in_tPos;
    layout(location = 2) in vec3 in_vNormal;

    out     vec3    vPos_orig;
    out     vec3    vPos;
    out     vec2    tPos;
    out     vec3    vNormal;

    void main(){
        vPos_orig = in_vPos;

        if( 0 != SkyboxMode ){
            // Skybox drawing
            gl_Position = vec4(in_vPos.x, in_vPos.y, 0.9999999f, 1.0f); // z: skybox is as far away as possible (at the far end of the [0,1) depth spectrum)
                                                                        //      more precision to 1.0f kills the skybox
        }else{
            // normal drawing
            //gl_Position = PVM * vec4(in_vPos, 1.0f);
            gl_Position = frameInfo.PV * M * vec4(in_vPos, 1.0f);

            gl_Position.z *= -1; // TODO: fix depth value
                                 //   this is a dirty hotfix for a depth issue, as the View and Projection seem fine,
                                 //   but the depth value is the inverse of what is needed.
        }

        vPos = gl_Position.xyz; // TODO: needed?

        tPos    = in_tPos;
        vNormal = in_vNormal;
    }
)";


const char* DefaultGeometryShader = R"()";
const char* DefaultFragmentShader = R"(
    #version 330
    precision highp float;

    // ----- shader/FrameInfo.ubo -----
    // FrameInfo v0.1
    layout(std140) uniform FrameInfo{
        // Row and column-major notations and clarifications in the GL specs are awful, they only serve to confuse.
        // C memory layout is row-oriented, but the value layout in it is column-major!
        // Don't get deceived by the 'layout(row_major)' definitions for matrices.
        // Both in C structs and the shader code, all matrices are column-major !!! (P * V * M) !!!
                          float t;          // current time
                          float dt;         // delta time since last frame
        layout(row_major) mat4  V;          // View matrix
        layout(row_major) mat4  Vrot;       // View matrix rotation component (without translation)
        layout(row_major) mat4  P;          // Projection matrix
        layout(row_major) mat4  PV;         // P * V
        layout(row_major) mat4  PVrotInv;   // inv(Vrot) * inv(P)
    } frameInfo;
    // --------------------------------

    uniform mat4 M;         // Model matrix
    uniform mat4 PVM;       // P * V * M

    uniform Lighting{
        vec3 ambient;

    } lighting;

    uniform int         WireframeMode;
    uniform vec3        WireframeColor;
    uniform int         MissingTexture;
    uniform int         AxisDrawMode;
    uniform int         SkyboxMode;
    uniform int         ColorMode;
    uniform vec3        Color;

    uniform vec3        LightAmbient;

    uniform sampler2D   gSampler;

    float pi    = 3.14159265358979323846;
    float t     = frameInfo.t;
    float dt    = frameInfo.dt;

    float nearZ = 1.0f;
    float farZ  = 100000.0f;


    in      vec3    vPos_orig;
    in      vec3    vPos;
    in      vec2    tPos;
    in      vec3    vNormal;
    out     vec4    FragColor;

    vec4 SampleMissingTexture( vec2 texPos ){
        int x = int(texPos.x*4) % 4;
        int y = int(texPos.y*4) % 4;
        int isBrightField = int(0 == (x + y) % 2);
        vec3 fieldColor = vec3( 1.0f*isBrightField, 0.0f, 1.0f*isBrightField ); // purple-black squares
        return vec4( fieldColor , 1.0f );
    }

    void main(){
        vec4 texel;
        if( 0 != MissingTexture ){                  // texture is missing, draw fallback texture
            texel = SampleMissingTexture( tPos );
        }else{                                       // draw texture normally
            texel = texture( gSampler, tPos );
        }

        //FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f ); //white
        //FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f ); //red
        //FragColor = vec4( 0.0f, 1.0f, 0.0f, 1.0f ); //green
        //FragColor = vec4( 0.0f, 0.0f, 1.0f, 1.0f ); //blue
        //FragColor = vec4( 1.0f, 0.0f, 1.0f, 1.0f ); //purple
        //FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f ); //yellow
        //FragColor = vec4( 0.0f, 1.0f, 1.0f, 1.0f ); //cyan

        //FragColor = vec4( texel.xyz * LightAmbient, texel.w);

        if( 0 != SkyboxMode ){
            //vec4 pixelVectorScreen = vec4( vPos.x, vPos.y, -1.0f, 1.0f );
            vec4 pixelVectorScreen = vec4( vPos.x, vPos.y, -vPos.z, 1.0f ); // -Z needed here, as the Projection matrix inverts Z with the -1 component in [3][2]
            vec4 pixelVectorWorld  = frameInfo.PVrotInv * pixelVectorScreen;
            pixelVectorWorld = normalize(pixelVectorWorld);

            float MyPitch = asin( pixelVectorWorld.z );                     //  [-pi/2; pi/2] // |input| must be < 1
            float MyYaw = atan( pixelVectorWorld.y, pixelVectorWorld.x );   //  [-pi; pi]

            float MyU = 0.5 - ( MyYaw /pi ) /2;
            float MyV = 0.5 + ( MyPitch * 2/pi) /2;
            vec2 MyUV = vec2( MyU, MyV );
            FragColor = texture( gSampler, MyUV );
        }else if( 0 == WireframeMode && 0 != AxisDrawMode){  // skip drawing axes without wireframe mode
            discard;
        }else if( 0 != WireframeMode && 0 == AxisDrawMode ){ // when drawing a non-axis in wireframe mode
            FragColor = vec4( WireframeColor.xyz, 1.0f );
        }else if( 0 != WireframeMode && 0 != AxisDrawMode ){ // when drawing an axis in wireframe mode
            FragColor = vec4( 1.0f * int( 0.0000001f < vPos_orig.x ),
                              1.0f * int( 0.0000001f < vPos_orig.y ),
                              1.0f * int( 0.0000001f < vPos_orig.z ),
                              1.0f );
        }else if( 0 != ColorMode ){             // drawing a fixed-color surface
            if( 1 == ColorMode ){
                FragColor = vec4( Color.xyz, 1.0f ); // draw a fix color
            }else{
                vec4 pulsingColor = vec4( 1+ sin( t  )/2,
                                          1+ sin( t*6 +pi/3 )/2,
                                          1+ sin( t*9 +pi/7 )/2,
                                          1.0f );
                FragColor = pulsingColor;
            }
        }else{                                  // drawing a textured surface normally
            vec3 totalLightColor = LightAmbient;
            totalLightColor = vec3( 1,1,1 );

            // visualize depth instead
            bool drawDepth = false;
            if( drawDepth ){
                float depth = gl_FragCoord.z /gl_FragCoord.w /(farZ-nearZ);
                FragColor = vec4( vec3( depth), 1.0f );
            }else{
                // output color
                FragColor = vec4( texel.x * totalLightColor.x,
                                  texel.y * totalLightColor.y,
                                  texel.z * totalLightColor.z,
                                  texel.w );
            }
        }
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


Config Engine::
GetConfig() const
{
    return stCfg;
}


uint32_t Engine::
GetCurrentTime() const
{
    return stLastTickTime;
}


SDL_Window* Engine::
GetMainWindow()
{
    return stMainSDLWindow;
}


math::int2 Engine::
GetMainWindowDimensions()
{
    return math::int2( stDefaultResWidth, stDefaultResHeight );
}


math::int2 Engine::
GetMainWindowPosition()
{
    math::int2 res;
    SDL_GetWindowPosition( stMainSDLWindow, &res.x, &res.y);
    return res;
}


std::string Engine::
ResolveMediaFilePath( const std::string& str )
{
    if( 0 == str.length() ){
        return std::string();
    }
    return std::string( "../../media/" ) + str;
}


bool Engine::
Initialize()
{
    if( !stInitialized ){
        PT_LOG_OUT( "-----------------------------------------" );
        PT_LOG_OUT( "Initializing engine" );
        PT_LOG_OUT( "-----" );

        engine::Engine::nameVertexShader.Init();
        engine::Engine::nameFragmentShader.Init();
        engine::Engine::nameShaderProgram.Init();

        engine::Engine::nameT.Init();
        engine::Engine::nameDT.Init();
        engine::Engine::nameM.Init();
        engine::Engine::nameV.Init();
        engine::Engine::nameVrot.Init();
        engine::Engine::namePV.Init();
        engine::Engine::namePVM.Init();

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
EndMainLoop()
{
    mMainLoopActive = false;
}


void Engine::
OnStart()
{
    SDLApplication::OnStart();

    auto sysmgr = NewPtr<SystemManager>();
    sysmgr->Initialize();
    Services::SetSystemControl( sysmgr );

    PT_LOG_OUT( "----- software, system and hardware -----" );
    PT_LOG_OUT( "LibPNG:    " << sysmgr->GetLibPNGInfo() );
    PT_LOG_OUT( "Zlib:      " << sysmgr->GetZLibInfo() );
    PT_LOG_OUT( "LibAssimp: " << sysmgr->GetAssimpInfo() );
    //PT_LOG_OUT( "-----" );
    //PT_LOG_OUT( sysmgr->GetCPUInfo() );
    //PT_LOG_OUT( "-----" );
    //PT_LOG_OUT( sysmgr->GetGPUInfo() );
    PT_LOG_OUT( "---------------- OpenGL -----------------" );
    PT_LOG_OUT( sysmgr->GetGraphicsAPIInfo() );
    PT_LOG_OUT( "-------Platform-specific parameters------" );
    PT_LOG_OUT( sysmgr->GetPlatformSpecificParameters() );
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

    mAssetManager = NewPtr<AssetManager>();
    Services::SetAssetControl( mAssetManager );


    sdlc->SetMainWindow( stMainSDLWindow );
    SDL_ShowWindow( stMainSDLWindow );

    // set up Main Camera
    mCamera = NewPtr<CameraPerspective>( "MainCamera" );
    mCamera->SetRotation( math::FRotator( 0, 90, 0) ); // view along the Y axis
    Actor::RegisterTickFunction( mCamera, TickGroup::PREPHYSICS );
    mCamera->Spawn();
    Services::GetDrawingControl()->SetMainCamera( mCamera );
    Services::GetDrawingControl()->SetCurrentCamera( mCamera );


    // load main vertex and fragment shader source code
    ConstStdStringPtr vertexShaderSource    = NewPtr<const std::string>( DefaultVertexShader );
    ConstStdStringPtr fragmentShaderSource  = NewPtr<const std::string>( DefaultFragmentShader );

    //set up shaders
    mVertexShader   = NewPtr<gl::Shader>( nameVertexShader, gl::ShaderType::VERTEX_SHADER, vertexShaderSource );
    mFragmentShader = NewPtr<gl::Shader>( nameFragmentShader, gl::ShaderType::FRAGMENT_SHADER, fragmentShaderSource );
    mShaderProgram  = NewPtr<engine::StandardShaderProgram>( nameShaderProgram );
    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mFragmentShader );
    mShaderProgram->Link();
    mShaderProgram->Use();

    mDrawingManager->SetDefaultShaderProgram( mShaderProgram );
    mDrawingManager->SetCurrentShaderProgram( mShaderProgram );

    mUniT   = mShaderProgram->GetUniform<float>( nameT );
    mUniDT  = mShaderProgram->GetUniform<float>( nameDT );
    mUniRotationMatrix        = mShaderProgram->GetUniform<mat4>( nameVrot );
    mUniViewMatrix            = mShaderProgram->GetUniform<mat4>( nameV );
    mUniViewProjectionMatrix  = mShaderProgram->GetUniform<mat4>( namePV );
    mUniModelMatrix               = mShaderProgram->GetUniform<mat4>( nameM );
    mUniModelViewProjectionMatrix = mShaderProgram->GetUniform<mat4>( namePVM );

    mShaderProgram->SetUniform( mUniRotationMatrix, mCamera->GetLookAtMtx() );
    mShaderProgram->SetUniform( mUniViewMatrix, mCamera->GetViewMtx() );
    mShaderProgram->SetUniform( mUniViewProjectionMatrix, mCamera->GetProjMtx() * mCamera->GetViewMtx() );
    mShaderProgram->SetUniform( mUniModelMatrix, mat4::identity );
    mShaderProgram->SetUniform( mUniModelViewProjectionMatrix, mat4::identity );

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
{}


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
    stCfgPath = std::string("../../cfg/Engine.cfg");
    try{
        // Config
        stCfg.setPath( stCfgPath );

        CfgAddKey( stCfg, iDefaultResWidth );
        CfgAddKey( stCfg, iDefaultResHeight );

        stCfg.readF( stCfgPath );
        PT_LOG_OUT( "Loaded config: '" << stCfgPath << "'" );

        int width = stCfg.getI( iDefaultResWidth );
        int height = stCfg.getI( iDefaultResHeight );
        if( 0 < width && 0 < height ){
            stDefaultResWidth = width;
            stDefaultResHeight = height;
        }else{
            PT_LOG_ERR( "Invalid resolution loaded from config '"
                        << stCfgPath << "'. Using defaults instead ("
                        << stDefaultResWidth << ", " << stDefaultResHeight << ")." );
        }
    }catch(...){
        PT_LOG_ERR( "Failed to load config file '" << stCfgPath << "'. Resetting to defaults." );
    }


    // Logging
    //result &= pt::log::Initialize( "./", pt::log::AutoGenerateLogFileName() );

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
    PT_LOG_DEBUG( "  SUCCESS - SDL Subsystems" );
    stInitializationTime = SDL_GetTicks();
    atexit( SDL_Quit );
    CreateUserEventType();
    PT_LOG_DEBUG( "  SUCCESS - Custom UserEvent type" );

    //TODO: read default config from file in previous Init steps
    //  apply window parameters based on them...

    // create hidden SDL Window (needed in order to have an OpenGL context)
    stMainSDLWindow = SDL_CreateWindow( "Indicus Engine Main Window",
                                        32, 32,
                                        stDefaultResWidth, stDefaultResHeight,
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
        auto windowID = stMainSDLWindow;
        stMainSDLWindow = nullptr;
        PT_LOG_DEBUG( "Deleted SDL Window(" << windowID << ")" );
    } );
    PT_LOG_DEBUG( "  SUCCESS - Main SDL Window (" << stMainSDLWindow << ")" );

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
        stMainGLContext = nullptr;
        PT_LOG_DEBUG( "Deleted GL context" );
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


    gl::Enable( GL_DEPTH_TEST );
    gl::Enable( GL_CULL_FACE );
    gl::Disable( GL_DITHER );    // TODO: figure out, whether it's worth it (deprecated in GL 4.5+)

    gl::Texture2d::Initialize();

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
Update()
{
    SDL_Event ev;
    // handle new events/inputs/etc.
    while( SDL_PollEvent( &ev ) ){
        OnEvent( &ev );
    }

    Uint32 currentTime = SDL_GetTicks() - stInitializationTime;
    bool comp = ( stInitializationTime < stLastTickTime );
    Uint32 lastTickTime = comp * stLastTickTime + (!comp) * stInitializationTime;
    float ft = currentTime / 1000.0f;
    float fdt = (currentTime - lastTickTime) / 1000.0f;

    if( fdt < 0.0001f ){
        return;
    }
    stLastTickTime = currentTime;

    UpdateGameState_PreActorTick( ft, fdt );

    // Actor Tick+TickDependency [un]registrations get executed here
    mScheduler->ProcessPendingTasks();

    // Tick all Actors
    mScheduler->TickPrePhysics( ft, fdt );
    mScheduler->TickDuringPhysics( ft, fdt );
    mScheduler->TickPostPhysics( ft, fdt );

    UpdateGameState_PostActorTick( ft, fdt );

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
    if( nullptr == mShaderProgram ){
        return;
    }

    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetCurrentCamera();
    if( cam ){
        mShaderProgram->SetUniform( mUniRotationMatrix, mCamera->GetLookAtMtx() );
        mShaderProgram->SetUniform( mUniViewMatrix, mCamera->GetViewMtx() );
        mShaderProgram->SetUniform( mUniViewProjectionMatrix, mCamera->GetProjMtx() * mCamera->GetViewMtx() );
    }

    if( nullptr != dc ){
        dc->DrawScene( t, dt );
    }
}
