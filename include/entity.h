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

#include <vector>

#include "worldcomponent.h"



namespace PT{
namespace engine{

    enum class TickGroup{
        NO_GROUP = 0,
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
        static void RegisterEntity(Entity* subject);
        /**
         * @brief UnregisterEntity
         *          Unregisters the Entity and its Components from the Engine
         *          -removes any Tick registrations at the start of the next frame
         * @param e
         */
        static void UnregisterEntity(Entity* subject);

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
        TickGroup   _tick_group;
        float       _tick_interval;
        float       _tick_last;
        bool        _tick_registered;
        bool        _registered;
    public:
        static void RegisterTickFunction(Entity* subject, TickGroup group = TickGroup::DURINGPHYSICS);
        static void UnregisterTickFunction(Entity* subject);
        static void AddTickDependency(Entity* subject, Entity* dependency);
        static void RemoveTickDependency(Entity* subject, Entity* dependency);
        void registerComponents();
        bool isRegistered() const;
        bool isTickRegistered() const;
        void enableTick();
        void disableTick();
        bool isEnabled() const;
        void tickEntity(float t, float dt);

        /**
         * @brief setTickInterval
         * @param interval: unit (ms)
         * @throws 'std::out_of_range' on negative values
         */
        void setTickInterval(float interval);
        float getTickInterval() const;

        TickGroup getTickGroup() const;
    };

} }
