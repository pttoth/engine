/** -----------------------------------------------------------------------------
  * FILE:    Component.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Common functionality for objects, the engine can use.
  *           Each component represents a certain distinct functionality.
  *           Actors hold them together, combining their functions to represent
  *           an object in game with complex functionality.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Def.h"
#include "pt/macros.h"
#include <string>

namespace engine{

PT_FORWARD_DECLARE_CLASS( Actor )
PT_FORWARD_DECLARE_CLASS( Component )

class Component
{
public:
    Component( const std::string& name );
    Component( const Component& other ) = delete;
    Component( Component&& source );
    virtual ~Component();
    Component& operator=( const Component& other ) = delete;
    Component& operator=( Component&& source );
    bool operator==( const Component& other ) const = delete;

    const std::string& GetName() const;

    virtual void Spawn();
    virtual void Despawn();
            bool IsSpawned() const;

    virtual void Tick( float t, float dt );

    static std::string GenerateComponentName( const std::string& parentname, const std::string& componentname );

protected:
    virtual void OnSpawned() = 0;
    virtual void OnDespawned() = 0;
    virtual void OnTick( float t, float dt ) = 0;

private:
    bool mSpawned = false;
    const std::string mName; //TODO: use 'pt::Name' for this

};

} //end of namespace engine
