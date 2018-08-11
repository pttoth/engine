/** -----------------------------------------------------------------------------
  * FILE:    entity.h
  * AUTHOR:  pttoth - 2018.08.07.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common class for objects in the game.
  *           Entities are made up of Components, each representing a specific
  *           functionality. Entities can register to the engine, to have their
  *           tick() function called each frame, allowing the Entity to
  *           update its state throughout execution.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "worldcomponent.h"

#include <vector>

namespace pttoth{
namespace engine{

    enum class TickGroup{
        PREPHYSICS,
        DURINGPHYSICS,
        POSTPHYSICS,
    };

    class Entity{

        WorldComponent* _rootComponent;
        std::vector<Component*> _components;

//private functions
        int indexOfComponent(Component* const c) const;
    protected:
        virtual void tick(float t, float dt) = 0;
    public:
        /**
         * @brief RegisterEntity
         *          Registers the Entity and its Components to the Engine
         */
        static void RegisterEntity(Entity* e);
        /**
         * @brief UnregisterEntity
         *          Unregisters the Entity and its Components from the Engine
         *          -removes any Tick registrations at the start of the next frame
         * @param e
         */
        static void UnregisterEntity(Entity* e);

        virtual void OnRegister() = 0;
        virtual void OnUnregister() = 0;

        Entity();
        Entity(const Entity& other);
        Entity(Entity&& other);
        virtual ~Entity();
        Entity& operator=(const Entity &other);
        Entity& operator=(Entity &&other);
        bool operator==(const Entity &other)const;
//Entity functions
        void addComponent(Component* component);
        void removeComponent(Component* component);
        std::vector<Component*> getComponents();
        WorldComponent* getRootComponent();

//tick
    private:
        bool        _tick_enabled;
        float       _tick_interval;
        float       _last_tick;
        TickGroup   _tick_group;
        bool        _tick_registered;
        bool        _entity_registered;
    public:
        static void RegisterTickFunction(Entity* e);
        static void UnregisterTickFunction(Entity* e);
        static void AddTickDependency(Entity* subject, Entity* dependency);
        static void RemoveTickDependency(Entity* subject, Entity* dependency);
        bool isRegistered();
        void enableTick();
        void disableTick();
        bool isEnabled();
        void addTickDependency(Entity* e);
        void RemoveTickDependency(Entity* e);
        void tickEntity(float t, float dt);
        void setTickInterval(float interval);
        void setTickGroup(TickGroup tg);
        TickGroup getTickGroup() const;
    };

} }
