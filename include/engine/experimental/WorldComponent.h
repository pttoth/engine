/** -----------------------------------------------------------------------------
  * FILE:    WorldComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component, with a transform, representing its location in the world.
  *           WorldComponents can be arranged into a hierarchy, in a parent-child
  *           relationship. Each WorldComponent can only have one parent, but
  *           multiple children. The WorldComponent's transform is relative to the
  *           parent (if it has no parent, it is relative to the World)
  *          Components (as opposed to Actors) are NOT threadsafe!
  *           TODO: ?? Use owning Actor to lock component for outside manipulation ??
  *          Never add Actor A's Components as parents/children to Actor B's components.
  *           TODO: Use the <create this component> component for that.
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/experimental/Component.h"

#include "engine/experimental/Transform.h"

#include "pt/math.h"

#include <memory>
#include <vector>


namespace engine{
namespace experimental{

class WorldComponent;
using WorldComponentPtr  = std::shared_ptr< WorldComponent >;
using WorldComponentPtrW = std::weak_ptr< WorldComponent >;

class WorldComponent: public Component
{
public:
    WorldComponent( const std::string& name );
    WorldComponent( const WorldComponent& other );
    WorldComponent( WorldComponent&& source );

    virtual ~WorldComponent();

    WorldComponent& operator=( const WorldComponent& other ) = delete;
    WorldComponent& operator=( WorldComponent&& source ) = delete;

    bool operator==( const WorldComponent& other ) const = delete;

//functions
    void OnAddedToEntity( ComponentVisitor& visitor ) override;
    void OnRemovedFromEntity( ComponentVisitor& visitor ) override;

    void Tick( float t, float dt ) override;

    void Decouple() override;

    void SetParent( WorldComponent* parent );
    void RemoveParent();
    WorldComponent* GetParent();
    std::vector<WorldComponent*> GetChildren();

    const pt::math::float3    GetWorldPosition() const;
    //const pt::math::float4    getWorldOrientation() const;
    //const pt::math::float3    getWorldScale() const;
    const pt::math::float4x4  GetWorldTransform() const;

    void SetPosition( const pt::math::float3& pos );
    void SetOrientation( const pt::math::float4& orient );
    void SetScale( const pt::math::float3& scale );
    void SetRelativeTransform( const pt::math::float3& pos, const pt::math::float4& orient, const pt::math::float3& scale );

protected:
    virtual void OnSpawned() override;
    virtual void OnDespawned() override;

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
    pt::math::float3    mPos;
    pt::math::float4    mOrient;
    pt::math::float3    mScale;
    pt::math::float4x4  mTransform; //position relative to world

    WorldComponent*                 mParent = nullptr;
    std::vector<WorldComponent*>    mChildren;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered entity
    //onunregistered    should be in World/Game and called for every registered entity


};



} //end of namespace experimental
} //end of namespace engine
