/** -----------------------------------------------------------------------------
  * FILE:    Component.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common functionality for objects, the engine can use.
  *           Each component represents a certain distinct functionality.
  *           Entities hold them together, combining their functions to represent
  *           an object in game with complex functionality.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Ticker.h"

#include <string>

namespace engine {

namespace entity{
    class ComponentVisitor;
}


class Component: public Ticker
{
public:
    Component(const std::string& name);
    Component(const Component& other);
    Component(Component&& other) = default;
    virtual ~Component();
    Component& operator=(const Component &other) = delete;
    Component& operator=(Component &&other)      = delete;
    bool operator==(const Component &other)const = delete;

    virtual void OnAddedToEntity(entity::ComponentVisitor& visitor);
    virtual void OnRemovedFromEntity(entity::ComponentVisitor& visitor);

    virtual void Spawn();
    virtual void Despawn();

    const std::string& GetName() const;

    void enableTick();
    void disableTick();
    virtual void Tick(float t, float dt) = 0;

    virtual void RegisterTickFunction();

protected:
    virtual void OnSpawned() = 0;
    virtual void OnDespawned() = 0;

private:
    const std::string mName;

    bool mTickEnabled = false;
    float mTickInterval = 0.0f;
    Group mTickGroup = Group::DURINGPHYSICS;
    bool  mTickRegistered = false;


    // Ticker interface
public:
    virtual float GetTickInterval() const override;
    virtual Group GetTickGroup() const override;
    virtual bool IsTickEnabled() const override;
    virtual bool IsTickRegistered() const override;
};

}
