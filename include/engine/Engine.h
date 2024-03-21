/** -----------------------------------------------------------------------------
  * FILE:    Engine.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/SDLApplication.h"
#include "engine/EngineControl.h"

#include "engine/Camera.h"
#include "engine/DefaultShaderProgram.h"
#include "engine/DrawingManager.h"
#include "engine/SerialScheduler.h"
#include "engine/SystemManager.h"
#include "engine/World.h"
#include "engine/gl/Uniforms.hpp"
#include "pt/alias.h"
#include "pt/config.h"
#include "pt/event.hpp"
#include "pt/utility.hpp"
#include "SDL2/SDL.h"
#include <cstdint>
#include <functional>
#include <vector>


//-------------------------------------

namespace engine{

PT_FORWARD_DECLARE_CLASS( Engine )

class Engine: public SDLApplication,
              public EngineControl
{

public:
    static Uint32 CreateUserEventType();
    static Uint32 GetUserEventType();

    Engine();
    Engine( int const argc, char* argv[] );
    virtual ~Engine();

    bool            DeveloperMode() const override;
    void            DeveloperMode( bool value );
    virtual void    Execute() override;
    pt::Config      GetConfig() const override;
    uint32_t        GetCurrentTime() const override;
    SDL_Window*     GetMainWindow() override;
    static bool     Initialize();


protected:
    virtual void  EndMainLoop();

    WorldPtr            mWorld          = nullptr;
    DrawingManagerPtr   mDrawingManager = nullptr;
    SerialSchedulerPtr  mScheduler      = nullptr;
    SystemManagerPtr    mSystemManager  = nullptr;

    // handles one full iteration of the game engine loop
    virtual void Update();

    // handles the gamestate update step of the game engine loop
    virtual void UpdateGameState(float t, float dt) = 0;

    /**
     * @brief onStart
     *          called at the start of the program
     *          initialion should go here
     */
    virtual void OnStart() override;

    /**
     * @brief onExit
     *          called before exiting the program
     *          deinitializion, resource release should go here
     */
    virtual void OnExit() override;

    /**
     * @brief onEvent
     *          called on every SDL_Event in the system, these range
     *              from user input
     *              through system state notifications (battery, etc)
     *              to custom Game events (e.g.: the main game timer ticks)
     */
    virtual void OnEvent(SDL_Event* event) override;

    /**
     * @brief onShutdownSignal
     *          called when receiving a signal from outside the program,
     *            notifying of an imminent shutdown
     *          emergency procedures for a clean shutdown go here
     * @note  has to call signalShutdownReady() at the end to
     *          notify the App, that it is ready to shut down
     */
    virtual void OnShutdownSignal() override;

    /**
     * @brief onMouseButtonDown
     * @param x
     * @param y
     * @param button:       "SDL_BUTTON_LEFT" "_MIDDLE" "_RIGHT" "_X1" or "_X2"
                X1: Mouse4 ???  CHECK
                X2: Mouse5 ???  CHECK
     * @param clicks:
     * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
     */
    virtual void OnMouseButtonDown(int32_t x, int32_t y,
                                   uint8_t button, uint8_t clicks,
                                   uint32_t timestamp, uint32_t mouseid);
    virtual void OnMouseButtonUp(int32_t x, int32_t y,
                                 uint8_t button, uint8_t clicks,
                                 uint32_t timestamp, uint32_t mouseid);
    /**
     * @param x:         horizontal mouse position in window
     * @param y:         vertical   mouse position in window
     * @param x_rel:     horizontal mouse position relative to last position
     * @param y_rel:     vertical   mouse position relative to last position
     * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
     */
    virtual void OnMouseMotion(int32_t x, int32_t y,
                               int32_t x_rel, int32_t y_rel,
                               uint32_t timestamp, uint32_t mouseid);
    /**
     * @param x:         horizontal movement ( -left + right )
     * @param y:         vertical   movement ( -down + up    )
     * @param direction: platform dependent formats
     *                     SDL_MOUSEWHEEL_NORMAL or SDL_MOUSEWHEEL_FLIPPED
     *                     FLIPPED negates x and y values
     * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
     */
    virtual void OnMouseWheel(int32_t x, int32_t y,
                              uint32_t timestamp, uint32_t mouseid,
                              uint32_t direction);
    /**
     * @param keycode:   key id
     * @param keymod:    active modifier keys
     * @param repeat:    non-zero if this is a key repeat
     */
    virtual void OnKeyDown(SDL_Keycode keycode, uint16_t keymod,
                           uint32_t timestamp, uint8_t repeat);

    /**
     * @param keycode:   key id
     * @param keymod:    active modifier keys
     * @param repeat:    non-zero if this is a key repeat
     */
    virtual void OnKeyUp(SDL_Keycode keycode, uint16_t keymod,
                         uint32_t timestamp, uint8_t repeat);

    /**
     * @todo:   create signature...
     */
    virtual void OnTouchInputEvent();

    inline Uint32 GetUptime(){
        return SDL_GetTicks() - stInitializationTime;
    }

private:
    static pt::Config       stCfg;
    static std::string      stCfgPath;
    static bool             stInitialized;
    static Uint32           stInitializationTime;
    static Uint32           stLastTickTime;
    static SDL_Window*      stMainSDLWindow;
    static SDL_GLContext    stMainGLContext; // type is 'SDL_GLContext'
    static Uint32           stUserEventType;

    static int32_t          stDefaultResWidth;
    static int32_t          stDefaultResHeight;

    bool mDeveloperMode = true;
    bool mMainLoopActive = false;

    void Construct();
    static bool InitializeActorAndComponentData();
    static bool InitializePtlib();
    static bool InitializeSDL_GL();
    static bool InitializeServices();


//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------
public:
protected:
    static const pt::Name nameVertexShader;
    static const pt::Name nameFragmentShader;
    static const pt::Name nameShaderProgram;
    static const pt::Name nameWireframeMode;
    static const pt::Name nameT;
    static const pt::Name nameDT;
    static const pt::Name nameM;
    static const pt::Name nameV;
    static const pt::Name nameVrot;
    static const pt::Name namePV;
    static const pt::Name namePVM;

private:
    engine::DefaultShaderProgramPtr mShaderProgram;
    engine::gl::ShaderPtr           mVertexShader;
    engine::gl::ShaderPtr           mFragmentShader;
    engine::CameraPtr               mCamera;

    gl::Uniform<int>    mUniWireframeMode;
    gl::Uniform<float>  mUniT;
    gl::Uniform<float>  mUniDT;
    gl::Uniform<math::float4x4>  mUniModelMatrix;
    gl::Uniform<math::float4x4>  mUniRotationMatrix;
    gl::Uniform<math::float4x4>  mUniViewMatrix;
    gl::Uniform<math::float4x4>  mUniViewProjectionMatrix;
    gl::Uniform<math::float4x4>  mUniModelViewProjectionMatrix;

    void drawScene(float t, float dt);

};
}

