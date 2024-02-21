/** -----------------------------------------------------------------------------
  * FILE:    WorldComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component, with a transform, representing its location in the world.
  *           WorldComponents can be arranged into a hierarchy, in a parent-child
  *           relationship. Each WorldComponent can only have one parent, but
  *           multiple children. The WorldComponent's transform is relative to the
  *           parent (if it has no parent, it is relative to the World)
  *          Never add Actor A's Components as parents/children to Actor B's components.
  *
  *  IGNORE: Components (as opposed to Actors) are NOT threadsafe!
  *           TODO: ?? Use owning Actor to lock component for outside manipulation ??
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/Component.h"

#include "engine/Transform.h"

#include "pt/event.hpp"
#include "pt/math.h"

#include <memory>
#include <vector>


namespace engine{

class WorldComponent;
using WorldComponentPtr       = std::shared_ptr< WorldComponent >;
using ConstWorldComponentPtr  = std::shared_ptr< const WorldComponent >;
using WorldComponentWPtr      = std::weak_ptr< WorldComponent >;
using ConstWorldComponentWPtr = std::weak_ptr< const WorldComponent >;
using WorldComponentUPtr      = std::unique_ptr< WorldComponent >;
using ConstWorldComponentUPtr = std::unique_ptr< const WorldComponent >;

class WorldComponent: public Component
{
public:
    WorldComponent( const std::string& name );
    WorldComponent( const WorldComponent& other ) = delete;
    WorldComponent( WorldComponent&& source ) = delete;

    virtual ~WorldComponent();

    WorldComponent& operator=( const WorldComponent& other ) = delete;
    WorldComponent& operator=( WorldComponent&& source ) = delete;

    bool operator==( const WorldComponent& other ) const = delete;

//functions
    void Decouple() override;

    void SetParent( WorldComponent* parent );
    void RemoveParent();
    WorldComponent* GetParent();
    std::vector<WorldComponent*> GetChildren();

    const math::float3    GetPosition() const;
    const math::float4    GetOrientation() const;
    const math::float3    GetScale() const;
    const math::float4x4  GetTransform() const;

    const math::float3    GetWorldPosition() const;
    const math::float4    GetWorldOrientation() const;
    const math::float3    GetWorldScale() const;
    const math::float4x4  GetWorldTransform() const;

    void SetPosition( const math::float3& pos );
    void SetOrientation( const math::float4& orient );
    void SetScale( const math::float3& scale );
    void SetRelativeTransform( const math::float3& pos, const math::float4& orient, const math::float3& scale );

    pt::Event<WorldComponent*> EvOnTransformChanged;

protected:
    pt::EventTrigger<WorldComponent*> EvOnTransformChangedTrigger;

    void OnSpawned() override;
    void OnDespawned() override;
    //void OnTick( float t, float dt ) override;

private:
    void AddChild( WorldComponent* component );
    void RemoveChild( WorldComponent* component );

    /**
     * @brief refreshPosition
     *         Recalculates WorldComponent position
     * @param bBasedOnAbsolute:
     *         true: Changes relative transform data based on current absolute position
     *         false: Changes absolute transform data based on position relative to parent
     *         default = false
     */
    void RefreshTransform();
    math::float3    mPos;
    math::float4    mOrient;
    math::float3    mScale;
    math::float4x4  mTransform; //position relative to world

    WorldComponent*                 mParent = nullptr;
    std::vector<WorldComponent*>    mChildren;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered actor
    //onunregistered    should be in World/Game and called for every registered actor


};



} //end of namespace engine
