/** -----------------------------------------------------------------------------
  * FILE:    Component.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common functionality for objects, the engine can use.
  *           Each component represents a certain distinct functionality.
  *           They need to be registered/unregistered to the engine to be functional.
  *           Entities hold them together, combining their functions to represent
  *           the Entity in the engine.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <string>

namespace engine {

namespace entity{
    class ComponentVisitor;
}


class Component
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

    /**
     * @brief Spawn:
     */
    virtual void Spawn();
    virtual void Despawn();

    const std::string& GetName() const;

    void enableTick();
    void disableTick();
    virtual void tick(float t, float dt) = 0;

private:
    const std::string mName;

    bool mTickEnabled = false;

};

}
