/** -----------------------------------------------------------------------------
  * FILE:    Entity.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common class for objects in the game.
  *           Entities are made up of Components, each representing a specific
  *           functionality. Entities can register to the engine scheduler,
  *           to have their Tick() function called each frame, allowing the Entity
  *           to update its state as time passes.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Ticker.h"
#include "engine/Common.h"
#include "engine/PositionComponent.h"

#include <vector>
#include <string>

namespace engine{

class Entity;
class WorldComponent;
class DrawableComponent;

namespace entity{

/**
 * @brief ComponentVisitor class:
 *          Used to identify Component subclasses, when they are added (removed) to (from) an Entity
 */
class ComponentVisitor
{
    Entity*     mEntity;
    Component*  mComponent;

public:
    ComponentVisitor(Entity& entity, Component& component);
    ComponentVisitor(const ComponentVisitor& other) = delete;
    ComponentVisitor(ComponentVisitor&& other) = delete;
    virtual ~ComponentVisitor(){}
    ComponentVisitor& operator=(const ComponentVisitor &other) = delete;
    ComponentVisitor& operator=(ComponentVisitor &&other) = delete;
    bool operator==(const ComponentVisitor &other)const = delete;

    void AddWorldComponent();
    void RemoveWorldComponent();
};

} //end of namespace 'entity'



class Entity: public Ticker
{
    friend class entity::ComponentVisitor;

public:
    const std::string& GetName() const;
private:
    const std::string mName;

    PositionComponent          mRootComponent;
    std::vector<Component*>         mComponents;
    std::vector<WorldComponent*>    mWorldComponents;
//private functions
    int IndexOfComponent(Component* const c) const;
protected:
    //virtual void OnCreateContext() = 0;
    //virtual void OnDeleteContext() = 0;
public:
    Entity(const std::string& name);
    Entity(const Entity& other);
    Entity(Entity&& other) = delete;
    virtual ~Entity();
    Entity& operator=(const Entity &other) = delete;
    Entity& operator=(Entity &&other) = delete;
    bool operator==(const Entity &other) const;
//Entity functions
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    std::vector<Component*> GetComponents();
    const WorldComponent* GetRootComponent() const;
    WorldComponent* GetRootComponent();

    void Spawn();
    void Despawn();

    //virtual void CreateContext();
    //virtual void DeleteContext();

//tick
private:
    //TODO: check which of these should be const

    bool            mRegistered;

    bool            mTickEnabled    = true;
    Ticker::Group   mTickGroup      = Group::DURINGPHYSICS;
    float           mTickInterval   = 0.0f;
    float           mTickLast       = 0.0f;
    bool            mTickRegistered = false;


    void AddWorldComponent(WorldComponent* component);
    void RemoveWorldComponent(WorldComponent* component);
public:
    static void RegisterTickFunction(Entity* subject, Group group = Group::DURINGPHYSICS);
    static void UnregisterTickFunction(Entity* subject);
    static void AddTickDependency(Entity* subject, Ticker* dependency);
    static void RemoveTickDependency(Entity* subject, Ticker* dependency);

    /**
     * @brief SetTickInterval
     * @param interval: unit (ms)
     * @throws 'std::out_of_range' on negative values
     */
    static void SetTickInterval(Entity& subject, float interval);

    virtual float GetTickInterval() const override;
    virtual Ticker::Group GetTickGroup() const override;
    virtual bool IsTickEnabled() const override;
    virtual bool IsTickRegistered() const override;

    void EnableTick();
    void DisableTick();

    void TickEntity(float t, float dt); //TODO: remove




};

}
