/** -----------------------------------------------------------------------------
  * FILE:    worldcomponent.h
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

#include "component.h"

#include "pt/math.h"


namespace engine{

class WorldComponent: public Component{
    friend class Component;
public:
    WorldComponent();
    WorldComponent(const WorldComponent& other) = delete;
    WorldComponent(WorldComponent&& other) = delete;
    virtual ~WorldComponent();
    WorldComponent& operator=(const WorldComponent& other) = delete;
    WorldComponent& operator=(WorldComponent&& other) = delete;
    bool operator==(const WorldComponent &other) const;
//functions
    void spawn();
    void setParent(WorldComponent* parent, bool bKeepPosition = false);
    void removeParent(bool bKeepPosition = false);

    pt::math::float3    getPosition() const;
    pt::math::float4    getOrientation() const;
    pt::math::float3    getScale() const;
    pt::math::float4x4  getTransform() const;

    void setPosition(const pt::math::float3& pos);
    void setOrientation(const pt::math::float4& orient);
    void setScale(const pt::math::float3& scale);
    void setRelativeTransform(const pt::math::float3& pos, const pt::math::float4& orient, const pt::math::float3& scale);
protected:
    virtual void onSpawn() = 0;
private:
    static void RegisterWorldComponentParts(WorldComponent* component);
    static void UnregisterWorldComponentParts(WorldComponent* component);

    /**
     * @brief refreshPosition
     *         Recalculates WorldComponent position
     * @param bBasedOnAbsolute:
     *         true: Changes relative transform data based on current absolute position
     *         false: Changes absolute transform data based on position relative to parent
     *         default = false
     */
    void refreshPosition(bool bBasedOnAbsolute = false);
    pt::math::float3    mPos;
    pt::math::float4    mOrient;
    pt::math::float3    mScale;
    pt::math::float4x4  mTransform; //position relative to world

    WorldComponent*     mParent;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered entity
    //onunregistered    should be in World/Game and called for every registered entity
};
}
