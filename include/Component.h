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

class Component{
    static void RegisterComponentParts(Component* component);
    static void UnregisterComponentParts(Component* component);

    const std::string mName;

    bool mTickEnabled;
    bool mIsRegistered;
public:
    Component(const std::string& name);
    Component(const Component& other);
    Component(Component&& other) = default;
    virtual ~Component();
    Component& operator=(const Component &other) = delete;
    Component& operator=(Component &&other)      = delete;
    bool operator==(const Component &other)const = delete;

    static void RegisterComponent(Component* component);
    static void UnregisterComponent(Component* component);    

    const std::string& GetName() const;

    void enableTick();
    void disableTick();
    virtual void tick(float t, float dt) = 0;

    bool isRegistered();
    virtual void OnRegistered() = 0;
    virtual void OnUnregistered() = 0;

};

}
