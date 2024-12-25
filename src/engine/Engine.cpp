#include "engine/Engine.h"

#include "engine/experimental/SDLWrapper.h"
#include "engine/gl/GlWrapper.h"
#include "engine/StandardShaderProgram.h"
#include "engine/service/Renderer.h"
#include "engine/service/Scheduler.h"
#include "engine/service/SDLManager.h"
#include "engine/Services.h"
#include "engine/actor/CameraPerspective.h"
#include "engine/component/BillboardComponent.h"
#include "engine/component/Component.h"
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
    iDefaultResHeight,
    iWindowMode
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
int32_t         engine::Engine::stDefaultWindowMode     = 0;


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


const char* DefaultVertexShader = R"()";

const char* DefaultGeometryShader = R"()";
//#include "../media/shader/test/DefaultFragmentShader.fs"
const char* DefaultFragmentShader = R"()";







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
{
    if( nullptr != mShaderProgram ){
        //mShaderProgram->FreeVRAM();
    }

    if( nullptr != mVertexShader ){
        //mVertexShader->FreeVRAM();
    }

    if( nullptr != mFragmentShader ){
        //mFragmentShader->FreeVRAM();
    }

    if( nullptr != mCamera ){
        //mCamera->Despawn();
        //mCamera->DestroyRenderContext();
    }

    //-----

    if( nullptr != mWorld ){
        //mWorld->
    }

    if( nullptr != mAssetManager ){
        //mAssetManager->
    }

    if( nullptr != mRenderer ){
        //mRenderer->SetSkyboxTexture( nullptr );
    }

    if( nullptr != mScheduler ){
        //mScheduler->
    }

    if( nullptr != mSystemManager ){
        //mSystemManager->
    }

}


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


const bool Engine::
HasKeyboardFocus()
{
    return mHasKeyboardFocus;
}


const bool Engine::
HasMouseFocus()
{
    return mHasMouseFocus;
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


    //--------------------------------------------------
    Services::SetEngineControl( this );

    auto sdlc = NewPtr<SDLManager>();
    Services::SetSDLControl( sdlc );

    mRenderer = NewPtr<RendererGL3_3>();
    Services::SetRenderer( mRenderer );

    mWorld = NewPtr<World>();
    Services::SetWorld( mWorld );

    mScheduler = NewPtr<SerialScheduler>();
    Services::SetScheduler( mScheduler );

    //--------------------------------------------------
    mAssetManager = NewPtr<AssetManager>();
    Services::SetAssetControl( mAssetManager );

    // @TODO: initialize textures AFTER shaders!

    // setup fallback textures
    {
        mAssetManager->SetFallbackTexture( gl::Texture2d::GetFallbackTexture() );
        mAssetManager->SetFallbackMaterialTexture( gl::Texture2d::GetFallbackMaterialTexture() );

        auto textures = gl::Texture2d::GenerateUnicolorTextures();
        for( auto& t : textures ){
            t->LoadToVRAM();
            mAssetManager->AddTexture( t );
        }
    }

    // setup fallback material
    {
        std::string fallbackMaterialName = "FallbackMaterial";
        std::string fallbackMaterialData = R"(
strTexture0Diffuse=MissingMaterialFallback
strTexture0Normal=MissingMaterialFallback
strTexture0Specular=MissingMaterialFallback
strTexture1Diffuse=MissingMaterialFallback
strTexture1Normal=MissingMaterialFallback
strTexture1Specular=MissingMaterialFallback
strVertexShader=shader/DefaultVertexShader.vs
strGeometryShader=
strFragmentShader=shader/DefaultFragmentShader.fs
strShaderProgramName=MainShaderProgram
)";
        gl::MaterialPtr mat = gl::Material::CreateFromString( fallbackMaterialName, fallbackMaterialData );
        mAssetManager->SetFallbackMaterial( mat );
    }


    //--------------------------------------------------

    sdlc->SetMainWindow( stMainSDLWindow );
    SDL_ShowWindow( stMainSDLWindow );

    // set up Main Camera
    mCamera = NewPtr<CameraPerspective>( "MainCamera" );
    mCamera->SetRotation( math::FRotator( 0, 90, 0) ); // view along the Y axis
    Actor::RegisterTickFunction( mCamera, TickGroup::PREPHYSICS );
    mCamera->Spawn();
    Services::GetRenderer()->SetMainCamera( mCamera );
    Services::GetRenderer()->SetCurrentCamera( mCamera );


    // load main vertex and fragment shader source code
    //ConstStdStringPtr vertexShaderSource    = NewPtr<const std::string>( DefaultVertexShader );
    //ConstStdStringPtr fragmentShaderSource  = NewPtr<const std::string>( DefaultFragmentShader );

    //set up shaders
    //mVertexShader   = NewPtr<gl::Shader>( nameVertexShader, gl::ShaderType::VERTEX_SHADER, vertexShaderSource );
    //mFragmentShader = NewPtr<gl::Shader>( nameFragmentShader, gl::ShaderType::FRAGMENT_SHADER, fragmentShaderSource );

    const char* vs_filename = "shader/DefaultVertexShader.vs";
    const char* fs_filename = "shader/DefaultFragmentShader.fs";
    mAssetManager->LoadShader( vs_filename );
    mAssetManager->LoadShader( fs_filename );
    mVertexShader   = mAssetManager->GetShader( vs_filename );
    mFragmentShader = mAssetManager->GetShader( fs_filename );
    mVertexShader->Compile();
    mFragmentShader->Compile();

    mShaderProgram  = NewPtr<engine::StandardShaderProgram>( nameShaderProgram );
    mShaderProgram->AddShader( mVertexShader );
    mShaderProgram->AddShader( mFragmentShader );

    if( mVertexShader->IsCompiled() && mFragmentShader->IsCompiled() ){ // avoid double compilation attempt in case of compiler errors
        mShaderProgram->Link();
    }
    mShaderProgram->Use();
    mAssetManager->SetFallbackShaderProgram( mShaderProgram );

    mRenderer->SetDefaultShaderProgram( mShaderProgram );
    mRenderer->SetCurrentShaderProgram( mShaderProgram );

    mUniT    = mShaderProgram->GetUniform<float>( nameT );
    mUniDT   = mShaderProgram->GetUniform<float>( nameDT );
    mUniVrot = mShaderProgram->GetUniform<mat4>( nameVrot );
    mUniV    = mShaderProgram->GetUniform<mat4>( nameV );
    mUniPV   = mShaderProgram->GetUniform<mat4>( namePV );
    mUniM    = mShaderProgram->GetUniform<mat4>( nameM );
    mUniPVM  = mShaderProgram->GetUniform<mat4>( namePVM );

    mShaderProgram->SetUniform( mUniVrot, mCamera->GetLookAtMtx() );
    mShaderProgram->SetUniform( mUniV, mCamera->GetViewMtx() );
    mShaderProgram->SetUniform( mUniPV, mCamera->GetProjMtx() * mCamera->GetViewMtx() );
    mShaderProgram->SetUniform( mUniM, mat4::identity );
    mShaderProgram->SetUniform( mUniPVM, mat4::identity );

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


uint32_t Engine::
GetWindowModeSDLValue( int32_t cfg_value )
{
    switch( cfg_value ){
        case 0: return 0;
        case 1: return SDL_WINDOW_BORDERLESS;
        case 2: return SDL_WINDOW_FULLSCREEN;
    }
    return 0;
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
    stCfgPath = std::string("../../cfg/Engine.cfg");
    try{
        // Config
        stCfg.setPath( stCfgPath );

        CfgAddKey( stCfg, iDefaultResWidth );
        CfgAddKey( stCfg, iDefaultResHeight );
        CfgAddKey( stCfg, iWindowMode );

        // Exception might happen here
        stCfg.readF( stCfgPath );
        PT_LOG_OUT( "Loaded config: '" << stCfgPath << "'" );

        int width       = stCfg.getI( iDefaultResWidth );
        int height      = stCfg.getI( iDefaultResHeight );
        int windowmode  = stCfg.getI( iWindowMode );
        if( (windowmode < 0) || (2 < windowmode) ){
            PT_LOG_ERR( "Invalid window mode loaded from config '"
                        << stCfgPath << "'. Reverting to: 0 (Windowed)" );
            windowmode = 0;
        }
        stDefaultWindowMode = windowmode;

        if( 0 < width && 0 < height ){
            stDefaultResWidth   = width;
            stDefaultResHeight  = height;
        }else{
            PT_LOG_ERR( "Invalid resolution loaded from config '"
                        << stCfgPath << "'. Using defaults instead ("
                        << stDefaultResWidth << ", " << stDefaultResHeight << ")." );
        }
    }catch(...){
        PT_LOG_ERR( "Failed to load config file '" << stCfgPath << "'. Resetting to defaults."
                    "\n  Wrong working directory at startup?"
                    "\n  Try running with the startup script next to the binary." );
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

    uint32_t flagWindowMode = Engine::GetWindowModeSDLValue( stDefaultWindowMode );

    // create hidden SDL Window (needed in order to have an OpenGL context)
    stMainSDLWindow = SDL_CreateWindow( "Indicus Engine Main Window",
                                        32, 32,
                                        stDefaultResWidth, stDefaultResHeight,
                                        SDL_WINDOW_HIDDEN
                                        | SDL_WINDOW_OPENGL
                                        | flagWindowMode
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
        PT_LOG_INFO( "Deleted SDL Window(" << windowID << ")" );
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

    RenderScene( ft, fdt );
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
    case SDL_WINDOWEVENT:
        switch( ev.window.event ){
            case SDL_WINDOWEVENT_ENTER:         mHasMouseFocus    = true;   break;
            case SDL_WINDOWEVENT_LEAVE:         mHasMouseFocus    = false;  break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:  mHasKeyboardFocus = true;   break;
            case SDL_WINDOWEVENT_FOCUS_LOST:    mHasKeyboardFocus = false;  break;
        }
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
RenderScene( float t, float dt )
{
    if( nullptr == mShaderProgram ){
        return;
    }

    auto dc = Services::GetRenderer();
    auto cam = dc->GetCurrentCamera();
    if( cam ){
        mShaderProgram->SetUniform( mUniVrot, mCamera->GetLookAtMtx() );
        mShaderProgram->SetUniform( mUniV, mCamera->GetViewMtx() );
        mShaderProgram->SetUniform( mUniPV, mCamera->GetProjMtx() * mCamera->GetViewMtx() );
    }

    if( nullptr != dc ){
        dc->DrawScene( t, dt );
    }
}
