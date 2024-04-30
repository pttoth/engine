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

    uniform int         DrawingAxes;
    uniform int         SkyboxMode;
    uniform float       t;
    uniform float       dt;
    uniform mat4        M;
    uniform mat4        V;
    uniform mat4        Vrot;
    uniform mat4        PV;
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
            gl_Position = vec4(in_vPos.x, in_vPos.y, 0.0f, 1.0f);
        }else{
            gl_Position = PVM * vec4(in_vPos, 1.0f);
        }

        vPos = gl_Position.xyz;

        tPos    = in_tPos;
        vNormal = in_vNormal;
    }
)";


const char* DefaultGeometryShader = R"()";
const char* DefaultFragmentShader = R"(
    #version 330
    precision highp float;

    uniform int         WireframeMode;
    uniform vec3        WireframeColor;
    uniform int         MissingTexture;
    uniform int         DrawingAxes;
    uniform int         SkyboxMode;
    uniform int         ColorMode;
    uniform vec3        Color;
    uniform float       t;
    uniform float       dt;
    uniform mat4        M;
    uniform mat4        V;
    uniform mat4        Vrot;
    uniform mat4        PV;
    uniform mat4        PVM;

    uniform vec2        ViewAngles; // (yaw, pitch) angles

    uniform sampler2D   gSampler;

    uniform int     AmbientLight_UseAlphaOverride;
    uniform float   AmbientLight_Alpha;

    uniform vec3    LightAmbient;

    in      vec3    vPos_orig;
    in      vec3    vPos;
    in      vec2    tPos;
    in      vec3    vNormal;
    out     vec4    FragColor;

    vec4 SampleMissingTexture( vec2 texPos ){
        int xquadrant = int(texPos.x*4) % 4;
        int yquadrant = int(texPos.y*4) % 4;
        int colored = int(0 == (xquadrant + yquadrant) % 2);
        return vec4( 1.0f*colored, 0.0f, 1.0f*colored, 1.0f );
    }

    void main(){
        vec4 texel = texture( gSampler, tPos );
        //FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f ); //white
        //FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f ); //red
        //FragColor = vec4( 0.0f, 1.0f, 0.0f, 1.0f ); //green
        //FragColor = vec4( 0.0f, 0.0f, 1.0f, 1.0f ); //blue
        //FragColor = vec4( 1.0f, 0.0f, 1.0f, 1.0f ); //purple
        //FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f ); //yellow
        //FragColor = vec4( 0.0f, 1.0f, 1.0f, 1.0f ); //cyan

        //FragColor = vec4( texel.xyz * LightAmbient, texel.w);

        float pi = 3.1415f;

        if( 0 != SkyboxMode ){
            //radius is 1.0f
            float phi0    = 0.0f; // horizon (vertical center)
            float lambda0 = 0.0f; // meridian (horizontal center)

            float FOV_angle = 75.0f;
            float FOV = pi/180 * 75.0f;

            float phi     = ViewAngles.y + (FOV/2 * vPos.y );
            float lambda  = ViewAngles.x + (FOV/2 * 16.0f/9.0f * vPos.x );



            //float phi     = ViewAngles.y; // (2*pi) / (ViewAngles.y/(2*pi)) ; // latitudinal position (vertical position along the edge of sphere)
            //float lambda  = ViewAngles.x;                                     // longitudinal position (horizontal position along the edge of sphere)

            float phi1    = cos( 0.5f ); // the standard parallels, where the scale of the projection is true

            //vec2 skyUV = vec2( (lambda - lambda0) * cos(phi1),
            //                   (phi - phi0) );
            vec2 skyUV = vec2( 1 * cos( ViewAngles.y ) * cos( ViewAngles.x ),
                              1* cos( 0.0f ) );

            //FragColor = texture( gSampler, skyUV );
            //FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );

            float texViewPortW = FOV * (16.0f/9.0f);
            float texViewPortH = FOV;// * (9.0f/16.0f);

            float viewPortU = (pi   + ViewAngles.x ) /(2*pi);
            float viewPortV = (pi/2 + ViewAngles.y ) /(pi);

            float u = viewPortU + (vPos.x - 0.5f) * texViewPortW /(2*pi);// * tan(ViewAngles.y - pi );
            float v = viewPortV + (vPos.y - 0.5f) * texViewPortH /(pi);

            //float u = viewPortU + atan(vPos.x)*texViewPortW / (2*pi);// *cos(ViewAngles.y)   ;// / texViewPortW );
            //float v = viewPortV + atan(vPos.y)*texViewPortH / pi;// / texViewPortH );

  //          float azimuth = acos( vPos.x ); // assumes |NearClippingPane| = 1.0f
  //          float elevation = asin( vPos.y ); // assumes |NearClippingPane| = 1.0f
//            FragColor = texture( gSampler, vec2( u,v ) );


            vec4 unitX = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
            vec4 unitY = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
            vec4 unitZ = vec4( 0.0f, 0.0f, 1.0f, 1.0f );
            mat4 fixedVrot = mat4( Vrot[0],
                                   Vrot[2],
                                   Vrot[1] * -1,
                                   Vrot[3] );

            vec3 pixelDirOrig = normalize( vec3( vPos.x, 1.0f, vPos.y ) );  // unitvector from viewPos to pixel
            //vec3 pixelDirOrig = unitY.xyz + vPos.x * unitX.xyz;
            //vec4 pixelDir     = Vrot * vec4(pixelDirOrig, 1.0f);            // camera-rotated unitvector to pixel (no need to normalize)
            vec4 pixelDir     = fixedVrot * vec4(pixelDirOrig, 1.0f);            // camera-rotated unitvector to pixel (no need to normalize)
                                                                            // since it is a unitvector, the tip is on the edge of the unit-sphere
            pixelDir     =  pixelDir.xzyw;
            pixelDir.z   *= -1;
            vec4 pixelDirProjXY = normalize( vec4( pixelDir.xy, 0.0f, 1.0f ) );

            float yaw   = acos( dot( unitY, pixelDirProjXY ) );
            float pitch = acos( dot( pixelDirProjXY, pixelDir ) );

            vec2 texUV = vec2( pi + yaw/(2*pi), pi/2 + pitch/pi );
            FragColor = texture( gSampler, texUV );
            //FragColor = texture( gSampler, vPos.xy );
            //FragColor = texture( gSampler, pixelDir.xy );



        }else if( 0 == WireframeMode && 0 != DrawingAxes){  // skip drawing axes without wireframe mode
            discard;
        }else if( 0 != WireframeMode && 0 == DrawingAxes ){ // when drawing wireframe without axes
            FragColor = vec4( WireframeColor.xyz, 1.0f );
        }else if( 0 != WireframeMode && 0 != DrawingAxes ){ // when drawing wireframe with axes
            FragColor = vec4( 1.0f * int( 0.01f < vPos_orig.x ),
                              1.0f * int( 0.01f < vPos_orig.y ),
                              1.0f * int( 0.01f < vPos_orig.z ),
                              1.0f );
        }else if( 0 != ColorMode ){
            if( 1 == ColorMode ){
                FragColor = vec4( Color.xyz, 1.0f ); // draw a fix color
            }else{
                vec4 pulsingColor = vec4( 1+ sin( t  )/2,
                                          1+ sin( t*6 +pi/3 )/2,
                                          1+ sin( t*9 +pi/7 )/2,
                                          1.0f );
                FragColor = pulsingColor;
            }
        }else{
            if( 0 != MissingTexture ){
                FragColor = SampleMissingTexture( tPos );
            }else{
                FragColor = vec4( texel.xyz, texel.w );
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
    mCamera->SetOrientation( math::FRotator( M_PI /2 , 0.0f, 0.0f) ); // view along the Y axis
    Actor::RegisterTickFunction( mCamera, TickGroup::PREPHYSICS );
    mCamera->Spawn();
    Services::GetDrawingControl()->SetMainCamera( mCamera );

    // load main vertex and fragment shader source code
    gl::ConstStdSharedPtr vertexShaderSource    = NewPtr<const std::string>( DefaultVertexShader );
    gl::ConstStdSharedPtr fragmentShaderSource  = NewPtr<const std::string>( DefaultFragmentShader );

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

    mShaderProgram->SetUniform( mUniRotationMatrix, mCamera->GetRotationMtx() );
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
        stMainSDLWindow = nullptr;
        PT_LOG_DEBUG( "Deleted SDL Window" );
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
    if( nullptr == mShaderProgram ){
        return;
    }

    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetMainCamera();
    if( cam ){
        mShaderProgram->SetUniform( mUniRotationMatrix, mCamera->GetRotationMtx() );
        mShaderProgram->SetUniform( mUniViewMatrix, mCamera->GetViewMtx() );
        mShaderProgram->SetUniform( mUniViewProjectionMatrix, mCamera->GetProjMtx() * mCamera->GetViewMtx() );
    }

    if( nullptr != dc ){
        dc->DrawScene( t, dt );
    }
}
