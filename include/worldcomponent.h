/** -----------------------------------------------------------------------------
  * FILE:    worldcomponent.h
  * AUTHOR:  pttoth
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
    WorldComponent(const WorldComponent& other);
    WorldComponent(WorldComponent&& other);
    virtual ~WorldComponent();
    WorldComponent& operator=(const WorldComponent &other);
    bool operator==(const WorldComponent &other)const;
//functions
    void spawn();
    void setParent(WorldComponent* parent, bool bKeepPosition = false);
    void removeParent(bool bKeepPosition = false);

    pt::math::float3    getPosition() const;
    pt::math::float4    getOrientation() const;
    pt::math::float3    getScale() const;
    pt::math::float4x4  getTransform() const;

    void setPosition(pt::math::float3& pos);
    void setOrientation(pt::math::float4& orient);
    void setScale(pt::math::float3& scale);
    void setRelativeTransform(pt::math::float3& pos, pt::math::float4& orient, pt::math::float3& scale);
protected:
    virtual void onSpawn() = 0;
private:
    static void _RegisterWorldComponentParts(WorldComponent* component);
    static void _UnregisterWorldComponentParts(WorldComponent* component);

    /**
     * @brief refreshPosition
     *         Recalculates WorldComponent position
     * @param bBasedOnAbsolute:
     *         true: Changes relative transform data based on current absolute position
     *         false: Changes absolute transform data based on position relative to parent
     *         default = false
     */
    void _refreshPosition(bool bBasedOnAbsolute = false);
    pt::math::float3    _pos;
    pt::math::float4    _orient;
    pt::math::float3    _scale;
    pt::math::float4x4  _transform; //position relative to world

    WorldComponent* _parent;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered entity
    //onunregistered    should be in World/Game and called for every registered entity
};
}
