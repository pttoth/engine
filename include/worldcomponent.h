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

//#include "transform2d.h"

#include "math/math.h"

namespace PT{
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

    math::float3    getPosition() const;
    math::float4    getOrientation() const;
    math::float3    getScale() const;
    math::float4x4  getTransform() const;

    void setPosition(math::float3& pos);
    void setOrientation(math::float4& orient);
    void setScale(math::float3& scale);
    void setRelativeTransform(math::float3& pos, math::float4& orient, math::float3& scale);
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
    math::float3    _pos;
    math::float4    _orient;
    math::float3    _scale;
    math::float4x4  _transform; //position relative to world

    WorldComponent* _parent;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered entity
    //onunregistered    should be in World/Game and called for every registered entity
};
} }
