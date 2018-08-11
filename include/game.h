/** -----------------------------------------------------------------------------
  * FILE:
  * AUTHOR:
  * EMAIL:
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "application.h"
#include "gamecontrol.h"
//#include "world.h"

#include "entity.h"
#include "SDL2/SDL.h"

#include <cstdint>
#include <vector>

#include "alg/std_extension.hpp"

//-------------------------------------

namespace pttoth{ namespace engine{

    class Game: public Application,
                public GameControl{

    public:
        Game();
        Game(int const argc, char* argv[]);
        ~Game();
    // GameControl interface
        void registerEntity(Entity *e);
        void unregisterEntity(Entity *e);
        void registerComponent(Component *c);
        void unregisterComponent(Component *c);
    protected:
        SDL_Window* window;
        SDL_Renderer* renderer;

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

        inline Uint32 getUpTime(){ return _uptime ; }
    private:
        Uint32 _uptime;
        Uint32 _tickrate = 50;
        SDL_TimerID _gametimer_id;
        std::vector<Entity*>    _entities;
        std::vector<Component*> _components;

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
         *          may throw std::logic_exception there, if:
         *          -bKeepUnusedDependencies is false
         *           and 'dependency' is not contained in 'subject's TickGroup
         * @throws std::logic_exception if:
         *          - 'subject' and 'dependency' are in different TickGroups
         */
        virtual void addTickDependency(Entity* subject, Entity* dependency) override;

        /**
         * @brief removeTickDependency:
         *          Removes ensurance, that 'subject' will only tick after 'dependecy' has ticked in the same TickGroup
         * @note  only takes effect at the start of the next frame
         * @throws std::logic_exception if:
         *          - 'subject' and 'dependency' are in different TickGroups
         */
        virtual void removeTickDependency(Entity* subject, Entity* dependency) override;
    protected:

    private:
//inner structs
        struct PendingTickReg{
            enum class Task{
                NO_TASK = 0,
                REGISTER_TICK,
                UNREGISTER_TICK,
                REGISTER_TICK_DEPENDENCY,
                UNREGISTER_TICK_DEPENDENCY,
                REMOVE_ENTITY_DEPENDENCIES,
            };
            Entity*     entity; //dependent entity
            TickGroup   group;  //entity TickGroup
            Task        task;   //task, to do with the entry
            Entity*     param; //used only with dependency
            PendingTickReg(Entity* e, TickGroup g, Task t, Entity* p = nullptr):
                                        entity(e), group(g), task(t), param(p){
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
                                           active(a),
                                           entity(e),
                                           ticked(false){
            }
            TickDependencyData(const TickDependencyData& other) = default;
            TickDependencyData(TickDependencyData&& source) = default;
            TickDependencyData& operator=(const TickDependencyData&other)= default;
            TickDependencyData operator=(TickDependencyData&& source) = default;
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

//variables
        bool bKeepUnusedDependencies = false;   //should call clearUnusedTickData when setting to false

//functions
        void tickThisGroupContainer(std::vector<TickDependencyData>& container, float t, float dt);
        std::vector<TickDependencyData>& getTickGroupContainer(TickGroup tg);
        void removeAllDependencies(Entity* e);

        std::vector<PendingTickReg> _tick_reg_pending;

        std::vector<TickDependencyData> _tick_prephysics;
        std::vector<TickDependencyData> _tick_duringphysics;
        std::vector<TickDependencyData> _tick_postphysics;

        /**
         * @brief clearUnusedTickData:
         *          Deletes all unused data from the tick registry
         * @note  this is only relevant if bKeepUnusedDependencies is true (or was at some point during execution)
         */
        void clearUnusedTickData();

        /**
         * @brief processTickRegistrationsPending:
         *          Executes all pending tick registrations
         * @note: this is called at the start of processing a new frame
         */
        void processTickRegistrationsPending();

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
         * @brief processTickDependencyCleanup:
         *          Removes any dependency references to the Entity given as parameter
         */
        void processTickDependencyCleanup(Entity* dependecy);

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
} }

