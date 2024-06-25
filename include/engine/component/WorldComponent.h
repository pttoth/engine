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

#include "Component.h"
#include "pt/event.hpp"
#include "pt/macros.h"
#include "pt/math.h"
#include <vector>

// OPTIMIZE: clean/dirty cached private mutable members (eg. Get$DIR() using 'GetRotationMtx()' )

namespace engine{

PT_FORWARD_DECLARE_CLASS( WorldComponent )

class WorldComponent: public Component
{
protected:
    pt::EventTrigger<WorldComponent*> EvOnTransformChangedTrigger;

public:
    pt::Event<WorldComponent*> EvOnTransformChanged;

//-----
    WorldComponent( const std::string& name );
    WorldComponent( const WorldComponent& other ) = delete;
    WorldComponent( WorldComponent&& source );

    virtual ~WorldComponent();

    WorldComponent& operator=( const WorldComponent& other ) = delete;
    WorldComponent& operator=( WorldComponent&& source );

    bool operator==( const WorldComponent& other ) const = delete;

// static functions
    static math::float3     ExtractPositionFromTransform( const math::float4x4& transform );
    static math::float4x4   ExtractRotationFromTransform( const math::float4x4& transform );
    static math::float3     ExtractScaleFromTransform( const math::float4x4& transform );
    static void             DissectTransform( math::float3* position, math::float4x4* rotation, math::float3* scale, const math::float4x4& transform );     //TODO: move this to 'math3d'
// functions

    math::float3  GetForward() const;
    math::float3  GetBackward() const;
    math::float3  GetRight() const;
    math::float3  GetLeft() const;
    math::float3  GetUp() const;
    math::float3  GetDown() const;

    void SetParent( WorldComponent* parent );
    void RemoveParent();
    WorldComponent* GetParent();
    std::vector<WorldComponent*> GetChildren();     // TODO: PERFORMANCE: avoid per-frame allocations!

    const math::float3    GetPosition() const;
    //const math::float4    GetOrientation() const;     //TODO: implement after having Quaternion class
    const math::float4x4  GetRotationMtx() const;
    const math::float3    GetScale() const;
    const math::float4x4  GetRelativeTransform() const;

    const math::float3    GetWorldPosition() const;
    //const math::float4    GetWorldOrientation() const;    // TODO: implement after having Quaternion class
    //const math::float4x4  GetWorldRotationMtx() const;    // TODO: remove, after implemented Quaternion
    //const math::float3    GetWorldScale() const;          // TODO: decide whether to remove...
    const math::float4x4  GetWorldTransform() const;        // TODO: optimize with a per-frame cache
                                                            //   this gets called a lot, multiple times, as every child calls up the whole chain

    void SetPosition( const math::float3& pos );
    //void SetOrientation( const math::float4& orient );    //TODO: implement after having Quaternion class
    void SetRotation( const math::FRotator& rotator );
    void SetRotation( const math::float4x4& rotation );
    void SetScale( const math::float3& scale );
    void SetRelativeTransform( const math::float3& pos, const math::FRotator& rotation, const math::float3& scale );
    void SetRelativeTransform( const math::float4x4& transform );

    void SetWorldPosition( const math::float3& pos );
    //void SetWorldOrientation( const math::float4& orient );    //TODO: implement after having Quaternion class
    void SetWorldRotation( const math::FRotator& rotator );
    //void SetWorldScale( const math::float3& scale );
    void SetWorldTransform( const math::float4x4& transform );

protected:
    void OnSpawned() override;
    void OnDespawned() override;
    //void OnTick( float t, float dt ) override;

    void RefreshTransform() const;

private:
    void AddChild( WorldComponent* component );
    void RemoveChild( WorldComponent* component );

    math::float3    mPos    = math::float3::zero;
    //math::float4    mOrient = math::float4( math::float4::zero );
    math::float4x4  mRotMtx = math::float4x4::identity;
    math::float3    mScale  = math::float3::one;
    mutable math::float4x4  mTransform = math::float4x4::identity;
    mutable bool mTransformDirty = false;

    // TODO: add cached WorldTransform and dirty flag for it

    WorldComponent*                         mParent = nullptr;
    mutable std::vector<WorldComponent*>    mChildren;
    //events
    //onPositionChanged
    //onregistered      should be in World/Game and called for every registered actor
    //onunregistered    should be in World/Game and called for every registered actor


};



} //end of namespace engine
