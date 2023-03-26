/** -----------------------------------------------------------------------------
  * FILE:    Engine.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/SDLApplication.h"
#include "engine/EngineControl.h"

#include "engine/DrawingManager.h"
#include "engine/SerialScheduler.h"
#include "engine/World.h"

#include "pt/alias.h"
#include "pt/config.h"
#include "pt/event.hpp"
#include "pt/utility.hpp"

#include "SDL2/SDL.h"

#include <cstdint>
#include <vector>
#include <functional>

//-------------------------------------

namespace engine{

class Engine: public SDLApplication,
              public EngineControl
{

public:
    static Uint32 GetUserEventType();

    //---------------------------------------------
    //threadsafe wrapper class for managing the game state update timer
    class GameTimer
    {
    public:
        struct State
        {
            Uint64      startTime = 0;        // 'timestamp' when timer was started
            Uint32      timerInterval = 0;    // delay between timer updates
            Uint32      tickInterval = 0;     // delay between game ticks
            Uint64      lastTimerUpdate = 0;  // 'timestamp' when last timer update occured
            Uint64      lastTick = 0;         // 'timestamp' when last tick occured
            Uint64      nextTick = 0;         // 'timestamp' when next tick will occur
            bool        timerActive = false;  // timer is started
        };

        static Uint32 TimerCallback( Uint32 interval, void* param );

        GameTimer();
        virtual ~GameTimer();

        bool    StartNewTimer( Uint32 tickInterval );
        bool    StopTimer();

        bool    IsRunning() const;
        Uint64  GetUptime() const;

        SDL_TimerID GetId() const;

        void    SetInterval( Uint32 interval );

        State   GetState() const;
        void    SetState( const State& state );

    private:
        bool    StopTimer_NoLock();
        bool    IsRunning_NoLock() const;

        mutable std::mutex  mMutex;

        // time at which the timer always reacts to changes
        static const Uint32 mMaximumResponseTime = 10; //ms

        SDL_TimerID mId = 0;
        State       mState;
    };
    //---------------------------------------------

    Engine();
    Engine(int const argc, char* argv[]);
    virtual ~Engine();

    void     SetTickrate( uint32_t rate ) const override;
    uint32_t GetTickrate() const override;


protected:
    SDL_Window*     mWindow     = nullptr;
    SDL_Renderer*   mRenderer   = nullptr;
    World           mWorld;
    DrawingManager  mDrawingManager;
    SerialScheduler mScheduler;
    GameTimer       mGameTimer;


    virtual void Update(float t, float dt) = 0;

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

    inline Uint32 GetUptime(){ return mUptime ; }

    enum eConfigKey{
        iTickRate,

    };
private:
    static Uint32    mUserEventType;

    float            mTickrate = 60.0f;
    Uint32           mUptime = 0;
    SDL_TimerID      mGametimerId = 0;

    pt::Config       mCfg;
    std::string      mCfgPath;


    void Construct();
    void InitializeConfig();
    void SetDefaultSettings();
    bool ReadConfig();
    void ProcessGameTimerEvent();   //TODO: remove

//--------------------------------------------------
//--------------------------------------------------
//                  Draw Handling
//--------------------------------------------------
//--------------------------------------------------
    public:
    protected:
    private:
        void drawScene(float t, float dt);

};
}

