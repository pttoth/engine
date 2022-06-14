/** -----------------------------------------------------------------------------
  * FILE:    Engine.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "SDLApplication.h"
#include "EngineControl.h"
#include "World.h"
#include "DrawingManager.h"
#include "SerialScheduler.h"

#include "Entity.h"
#include "SDL2/SDL.h"

#include <cstdint>
#include <vector>

#include "pt/utility.hpp"
#include "pt/config.h"
#include "pt/event.hpp"

#include <functional>

//-------------------------------------

namespace engine{

class Engine: public SDLApplication,
              public EngineControl
{

public:
    Engine();
    Engine(int const argc, char* argv[]);
    virtual ~Engine();
// EngineControl interface
    virtual void RegisterEntity(Entity& entity) override;
    virtual void UnregisterEntity(Entity& entity) override;
    virtual void RegisterComponent(Component& component) override;
    virtual void UnregisterComponent(Component& component) override;

protected:
    SDL_Window*     mWindow;
    SDL_Renderer*   mRenderer;
    World           mWorld;
    DrawingManager  mDrawingManager;
    SerialScheduler mScheduler;

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

    inline Uint32 GetUpTime(){ return mUptime ; }

    enum eConfigKey{
        iTickRate,

    };
private:
    Uint32                  mUptime;    //TODO AddScheduler: remove
    Uint32                  mTickrate;  //TODO AddScheduler: remove
    SDL_TimerID             mGametimerId;
    pt::Config              mCfg;
    std::string             mCfgPath;
    std::vector<Entity*>    mEntities;
    std::vector<Component*> mComponents;

    void Construct();
    void InitializeConfig();
    void SetDefaultSettings();
    bool ReadConfig();
    void ProcessGameTimerEvent();   //TODO: remove

//--------------------------------------------------
//--------------------------------------------------
//                  Tick Handling
//--------------------------------------------------
//--------------------------------------------------
public:
    //TODO AddScheduler: remove these
    virtual void RegisterTick(Ticker& subject) override;
    virtual void UnregisterTick(Ticker& subject) override;
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) override;
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) override;
    virtual void RemoveEntityDependencies(Ticker& subject) override;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) override;
    //-----


protected:

private:
    //TODO AddScheduler: delete these
    pt::EventTrigger<> mPendingTasksTrigger;
    pt::Event<>        mPendingTasks;
    //-----


    //TODO AddScheduler: delete these
    //----------------------------------------------------------------------
    //TODO: dump this, when implementing graph traversal for resolving dependencies
    //----------------------------------------------------------------------
    struct TickDependencyData;
    struct TickDependencyData{
        Ticker* subject;
        std::vector<Ticker*> dependencies;
        bool active;
        bool ticked;
        TickDependencyData(Ticker* e, bool a = true):
            subject(e), active(a),
            ticked(false)
        {}
        TickDependencyData(const TickDependencyData& other) = default;
        TickDependencyData(TickDependencyData&& source) = default;
        TickDependencyData& operator=(const TickDependencyData&other)= default;
        TickDependencyData& operator=(TickDependencyData&& source) = default;
        //only checks if subject Entity is the same
        bool operator==(const TickDependencyData &other)const{
            return (subject == other.subject);
        }
        //only checks if subject Entity is the same
        bool operator==(const Ticker* e)const{
            return (subject == e);
        }
        //tells, whether the Entity should tick this frame
        bool shouldTick(){ return (active && !ticked); }
    };
    //-----

//TODO AddScheduler: delete these
//functions
    void TickElementsInGroupContainer(std::vector<TickDependencyData>& container, float t, float dt);
    std::vector<TickDependencyData>& GetTickGroupContainer(Ticker::Group tg);

    std::vector<TickDependencyData> mTickDepPrephysics;
    std::vector<TickDependencyData> mTickDepDuringphysics;
    std::vector<TickDependencyData> mTickDepPostphysics;
//-----

//TODO AddScheduler: delete these
    void ClearUnusedTickData();

    /**
     * @brief processTickRegistrationsPending:
     *          Executes all pending tick registrations
     * @note: this is called at the start of processing a new frame
     */
    void ProcessRegistrationsPending();


    void TickPrePhysics(float t, float dt);
    void TickDuringPhysics(float t, float dt);
    void TickPostPhysics(float t, float dt);
//-----


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

