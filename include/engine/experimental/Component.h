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

#include <string>
#include <memory>

//TODO: remove this
//namespace ns = engine;
//namespace ns = engine::experimental;

namespace engine{
namespace experimental{

class Actor;
class ComponentVisitor;

class Component;
using ComponentPtr  = std::shared_ptr< Component >;
using ComponentPtrW = std::weak_ptr< Component >;

class Component
{
public:
    Component( const std::string& name );
    Component( const Component& other );
    Component( Component&& source );
    virtual ~Component();
    Component& operator=( const Component& other ) = delete;
    Component& operator=( Component&& source ) = delete;
    bool operator==( const Component& other ) const = delete;

    const std::string& GetName() const;

    virtual void OnAddedToEntity( ComponentVisitor& visitor );
    virtual void OnRemovedFromEntity( ComponentVisitor& visitor );

    void Spawn();
    void Despawn();

    virtual void Tick( float t, float dt ) = 0;

    /**
     * @brief Decouple:
     *  Cleanly severes all references to linked objects and notifies
     *  them to also release their stored references to this object.
     */
    virtual void Decouple() = 0;

    static std::string GenerateComponentName( const std::string& parentname, const std::string& componentname );

protected:

    virtual void OnSpawned() = 0;
    virtual void OnDespawned() = 0;

private:

    const std::string mName; //TODO: use 'pt::Name' for this
};

} //end of namespace experimental
} //end of namespace engine