/** -----------------------------------------------------------------------------
  * FILE:    engine.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "sdlapplication.h"
#include "enginecontrol.h"
#include "world.h"

#include "entity.h"
#include "SDL2/SDL.h"

#include <cstdint>
#include <vector>

#include "pt/utility.hpp"
#include "pt/config.h"

//-------------------------------------

namespace engine{

class Engine: public SDLApplication,
              public EngineControl{

public:
    Engine();
    Engine(int const argc, char* argv[]);
    ~Engine();
// EngineControl interface
    virtual void registerEntity(Entity *e) override;
    virtual void unregisterEntity(Entity *e) override;
    virtual void registerComponent(Component *c) override;
    virtual void unregisterComponent(Component *c) override;
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;
    World world;


    virtual void tick(float t, float dt) = 0;

    /**
     * @brief onStart
     *          called at the start of the program
     *          initialion should go here
     */
    virtual void onStart() override;

    /**
     * @brief onExit
     *          called before exiting the program
     *          deinitializion, resource release should go here
     */
    virtual void onExit() override;

    /**
     * @brief onEvent
     *          called on every SDL_Event in the system, these range
     *              from user input
     *              through system state notifications (battery, etc)
     *              to custom Game events (e.g.: the main game timer ticks)
     */
    virtual void onEvent(SDL_Event* event) override;

    /**
     * @brief onShutdownSignal
     *          called when receiving a signal from outside the program,
     *            notifying of an imminent shutdown
     *          emergency procedures for a clean shutdown go here
     * @note  has to call signalShutdownReady() at the end to
     *          notify the App, that it is ready to shut down
     */
    virtual void onShutdownSignal() override;

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
    virtual void onMouseButtonDown(int32_t x, int32_t y,
                                   uint8_t button, uint8_t clicks,
                                   uint32_t timestamp, uint32_t mouseid);
    virtual void onMouseButtonUp(int32_t x, int32_t y,
                                 uint8_t button, uint8_t clicks,
                                 uint32_t timestamp, uint32_t mouseid);
    /**
     * @param x:         horizontal mouse position in window
     * @param y:         vertical   mouse position in window
     * @param x_rel:     horizontal mouse position relative to last position
     * @param y_rel:     vertical   mouse position relative to last position
     * @param mouseid:      id of mouse, or SDL_TOUCH_MOUSEID if touchpad
     */
    virtual void onMouseMotion(int32_t x, int32_t y,
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
    virtual void onMouseWheel(int32_t x, int32_t y,
                              uint32_t timestamp, uint32_t mouseid,
                              uint32_t direction);
    /**
     * @param keycode:   key id
     * @param keymod:    active modifier keys
     * @param repeat:    non-zero if this is a key repeat
     */
    virtual void onKeyDown(SDL_Keycode keycode, uint16_t keymod,
                           uint32_t timestamp, uint8_t repeat);

    /**
     * @param keycode:   key id
     * @param keymod:    active modifier keys
     * @param repeat:    non-zero if this is a key repeat
     */
    virtual void onKeyUp(SDL_Keycode keycode, uint16_t keymod,
                         uint32_t timestamp, uint8_t repeat);

    /**
     * @todo:   create signature...
     */
    virtual void onTouchInputEvent();

    inline Uint32 getUpTime(){ return mUptime ; }

    enum eConfigKey{
        iTickRate,

    };
private:
    Uint32                  mUptime;
    Uint32                  mTickrate;
    SDL_TimerID             mGametimerId;
    pt::Config              mCfg;
    std::string             mCfgPath;
    std::vector<Entity*>    mEntities;
    std::vector<Component*> mComponents;

    Camera*                 mMainCamera;

    void construct();
    void initializeConfig();
    void setDefaultSettings();
    bool readConfig();
    void processGameTimerEvent();

//--------------------------------------------------
//--------------------------------------------------
//                  Tick Handling
//--------------------------------------------------
//--------------------------------------------------
public:
    //GameControl interface
    /**
     * @brief registerTick:
     *          Registers Entity to have its tick() function called during frames
     * @note  only takes effect at the start of the next frame
     */
    virtual void registerTick(Entity *e) override;

    /**
     * @brief unregisterTick:
     *          Unregisters Entity to not have its tick() function called during frames
     * @note  only takes effect at the start of the next frame
     */
    virtual void unregisterTick(Entity *e) override;

    /**
     * @brief addTickDependency:
     *          Ensures, that 'subject' will only tick after 'dependecy' has ticked in the same TickGroup
     *          'subject' and 'dependency' has to be in the same TickGroup
     * @note  only takes effect at the start of the next frame
     */
    virtual void addTickDependency(Entity* subject, Entity* dependency) override;

    /**
     * @brief removeTickDependency:
     *          Removes ensurance, that 'subject' will only tick after 'dependecy' has ticked in the same TickGroup
     * @note  only takes effect at the start of the next frame
     */
    virtual void removeTickDependency(Entity* subject, Entity* dependency) override;

    virtual void removeEntityDependencies(Entity* subject) override;
    virtual void removeDependenciesReferencingEntity(Entity* dependency) override;

    virtual         void     SetMainCamera(Camera *camera) override;
    virtual const   Camera*  GetMainCamera() const override;
    virtual         Camera*  GetMainCamera() override;

protected:

private:
//inner structs
    struct PendingTask{
        enum class Task{
            NO_TASK = 0,
            REGISTER_ENTITY,
            UNREGISTER_ENTITY,
            REGISTER_COMPONENT,
            UNREGISTER_COMPONENT,
            REGISTER_TICK,
            UNREGISTER_TICK,
            REGISTER_TICK_DEPENDENCY,
            UNREGISTER_TICK_DEPENDENCY,
            REMOVE_ENTITY_DEPENDENCIES, //clears all dependencies for Entity
            REMOVE_DEPENDENCIES_REFERENCING_ENTITY, //clears all dependencies referencing Entity
        };
        Entity*     subject; //dependent entity
        Component*  subject_component;
        TickGroup   group;  //entity TickGroup
        Task        task;   //task, to do with the entry
        Entity*     dependency; //Entity, that 'subject' depends on
        PendingTask(Component* c, Task t):
            subject(nullptr), subject_component(c),
            group(TickGroup::NO_GROUP), task(t), dependency(nullptr){
        }
        PendingTask(Entity* e, Task t):
            subject(e), subject_component(nullptr),
            group(TickGroup::NO_GROUP), task(t), dependency(nullptr){
        }
        PendingTask(Entity* e, TickGroup g, Task t, Entity* p = nullptr):
            subject(e), subject_component(nullptr), group(g), task(t), dependency(p){
        }
    };

    //----------------------------------------------------------------------
    //TODO: dump this, when implementing graph traversal for solving dependencies
    //----------------------------------------------------------------------
    struct TickDependencyData;
    struct TickDependencyData{
        Entity* entity;
        std::vector<Entity*> dependencies;
        bool active;
        bool ticked;
        TickDependencyData(Entity* e, bool a = true):
            entity(e), active(a),
            ticked(false)
        {}
        TickDependencyData(const TickDependencyData& other) = default;
        TickDependencyData(TickDependencyData&& source) = default;
        TickDependencyData& operator=(const TickDependencyData&other)= default;
        TickDependencyData& operator=(TickDependencyData&& source) = default;
        //only checks if subject Entity is the same
        bool operator==(const TickDependencyData &other)const{
            return (entity == other.entity);
        }
        //only checks if subject Entity is the same
        bool operator==(const Entity* e)const{
            return (entity == e);
        }
        //tells, whether the Entity should tick this frame
        bool shouldTick(){ return (active && !ticked); }
    };

//functions
    void tickThisGroupContainer(std::vector<TickDependencyData>& container, float t, float dt);
    std::vector<TickDependencyData>& getTickGroupContainer(TickGroup tg);

    std::vector<PendingTask> _pending_tasks;

    std::vector<TickDependencyData> _tick_prephysics;
    std::vector<TickDependencyData> _tick_duringphysics;
    std::vector<TickDependencyData> _tick_postphysics;

    void processEntityRegister(Entity* subject);
    void processEntityUnregister(Entity* subject);
    void processComponentRegister(Component* subject);
    void processComponentUnregister(Component* subject);

    /**
     * @brief clearUnusedTickData:
     *          Deletes all unused data from the tick registry
     */
    void clearUnusedTickData();

    /**
     * @brief processTickRegistrationsPending:
     *          Executes all pending tick registrations
     * @note: this is called at the start of processing a new frame
     */
    void processRegistrationsPending();

    /**
     * @brief processTickRegister:
     *          Registers 'subject' to tick during 'group' phase when updating frames
     * @param subject:  Entity to register
     * @param group:    TickGroup to register to
     */
    void processTickRegister(Entity* subject, TickGroup group);

    /**
     * @brief processTickUnregister:
     *          Unregisters 'subject', so that it doesn't tick during 'group' phase when updating frames
     * @param subject:  Entity to unregister
     * @param group:    TickGroup to unregister from
     */
    void processTickUnregister(Entity* subject, TickGroup group);

    /**
     * @brief processTickDependencyRegister
     *          Adds references for 'dependency' in 'subject's dependecy data
     * @param subject:    Entity depending on 'dependency'
     * @param dependency: Entity 'subject' depends on
     */
    void processTickDependencyRegister(Entity* subject, Entity* dependency);

    /**
     * @brief processTickDependencyUnregister
     *          Removes references for 'dependency' in 'subject's dependecy data
     * @param subject:    Entity depending on 'dependency'
     * @param dependency: Entity 'subject' depends on
     */
    void processTickDependencyUnregister(Entity* subject, Entity* dependency);

    /**
     * @brief processTickDependencyRemoveAll
     *          Removes all dependencies for 'subject'
     */
    void processTickDependencyRemoveAll(Entity* subject);

    /**
     * @brief processTickDependencyReferenceCleanup:
     *          Removes any dependency references to 'dependecy'
     */
    void processTickDependencyReferenceCleanup(Entity* dependecy);

    /**
     * @brief tickPrePhysics:
     *          Ticks every Entity registered for PREPHYSICS
     * @param t:  current time
     * @param dt: time passed since frame
     */
    void tickPrePhysics(float t, float dt);

    /**
     * @brief tickPrePhysics:
     *          Ticks every Entity registered for DURINGPHYSICS
     * @param t:  current time
     * @param dt: time passed since frame
     */
    void tickDuringPhysics(float t, float dt);

    /**
     * @brief tickPrePhysics:
     *          Ticks every Entity registered for POSTPHYSICS
     * @param t:  current time
     * @param dt: time passed since frame
     */
    void tickPostPhysics(float t, float dt);

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

