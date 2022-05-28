/** -----------------------------------------------------------------------------
  * FILE:    WorldComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component, with a transform, representing its location in the world.
  *           WorldComponents can be arranged into a hierarchy, in a parent-child
  *           relationship. Each WorldComponent can only have one parent, but
  *           multiple children. The WorldComponent's transform is relative to the
  *           parent (if it has no parent, it is relative to the World)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "Component.h"

#include "pt/math.h"
#include <vector>


namespace engine{

class WorldComponent: public Component{
    friend class Component;
public:
    WorldComponent(const std::string& name);
    WorldComponent(const WorldComponent& other);
    WorldComponent(WorldComponent&& other) = delete;
    virtual ~WorldComponent();
    WorldComponent& operator=(const WorldComponent& other) = delete;
    WorldComponent& operator=(WorldComponent&& other) = delete;
    bool operator==(const WorldComponent &other) const;

    //virtual void OnRegistered() override;
    //virtual void OnUnregistered() override;

//functions
    virtual void OnAddedToEntity(entity::ComponentVisitor& visitor) override;
    virtual void OnRemovedFromEntity(entity::ComponentVisitor& visitor) override;
    virtual void Spawn() override;
    virtual void Despawn() override;
    void setParent(WorldComponent* parent);
    void removeParent();
    WorldComponent* GetParent();
    std::vector<WorldComponent*> GetChildren();

    const pt::math::float3    getPosition() const;
    const pt::math::float4    getOrientation() const;
    const pt::math::float3    getScale() const;
    const pt::math::float4x4  getTransform() const;

    const pt::math::float3    getWorldPosition() const;
    //const pt::math::float4    getWorldOrientation() const;
    //const pt::math::float3    getWorldScale() const;
    const pt::math::float4x4  getWorldTransform() const;

    void setPosition(const pt::math::float3& pos);
    void setOrientation(const pt::math::float4& orient);
    void setScale(const pt::math::float3& scale);
    void setRelativeTransform(const pt::math::float3& pos, const pt::math::float4& orient, const pt::math::float3& scale);
protected:
    virtual void OnSpawn() = 0;
    virtual void OnDespawn() = 0;
private:
    static void RegisterWorldComponentParts(WorldComponent* component);
    static void UnregisterWorldComponentParts(WorldComponent* component);

    void AddChild(WorldComponent* component);
    void RemoveChild(WorldComponent* component);


    /**
     * @brief refreshPosition
     *         Recalculates WorldComponent position
     * @param bBasedOnAbsolute:
     *         true: Changes relative transform data based on current absolute position
     *         false: Changes absolute transform data based on position relative to parent
     *         default = false
     */
    void refreshTransform();
    pt::math::float3    mPos;
    pt::math::float4    mOrient;
    pt::math::float3    mScale;
    pt::math::float4x4  mTransform; //position relative to world

    WorldComponent*              mParent;
    std::vector<WorldComponent*> mChildren;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered entity
    //onunregistered    should be in World/Game and called for every registered entity

};
}
