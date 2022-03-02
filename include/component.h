/** -----------------------------------------------------------------------------
  * FILE:    component.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common functionality for objects, the engine can use.
  *           Each component represents a certain distinct functionality.
  *           They need to be registered/unregistered to the engine to be functional.
  *           Entities hold them together, combining their functions to represent
  *           the Entity in the engine.
  *           When the Entity ticks, it calls its Components' tick functions.
  *           The Component uses it to update its state for the frame.
  * -----------------------------------------------------------------------------
  */

#pragma once

namespace engine {

class Component{
    static void _RegisterComponentParts(Component* component);
    static void _UnregisterComponentParts(Component* component);

    bool _tick_enabled;
    bool _registered;
public:
    Component();
    Component(const Component& other) = delete;
    Component(Component&& other) = default;
    virtual ~Component();
    Component& operator=(const Component &other) = delete;
    Component& operator=(Component &&other)      = delete;
    bool operator==(const Component &other)const = delete;

    static void RegisterComponent(Component* component);
    static void UnregisterComponent(Component* component);    

    void enableTick();
    void disableTick();
    virtual void tick(float t, float dt) = 0;

    bool isRegistered();
    virtual void OnRegistered() = 0;
    virtual void OnUnregistered() = 0;

};

}
