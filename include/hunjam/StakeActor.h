#pragma once

#include "engine/actor/Actor.h"


#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/BillboardComponent.h"
#include "engine/component/MeshComponent.h"
#include "engine/component/LightPointComponent.h"


#include "engine/Def.h"
#include "pt/macros.h"

namespace hunjam{

PT_FORWARD_DECLARE_CLASS( StakeActor )


class StakeActor: public engine::Actor
{
public:
    engine::AxisDisplayComponentPtr mAxis;
    engine::AxisDisplayComponentPtr mMeshAxis;
    engine::MeshComponentPtr        mMesh;
    engine::MeshComponentPtr        mMeshCollider;

    math::mat4 mOrientOverride = math::mat4::identity;

    bool    mIsFlying       = false;
    bool    mHasLanded      = false;
    bool    mIsReturning    = false;

    math::vec3          mPositionOffset = math::vec3( 100, 0, -25 );

    float               mThrowForce     = 100;
    math::vec3          mInertia;
    const math::vec3    mGravity        = math::vec3( 0, 0, -1.0f );

    uint64_t mLandTime = 0;

    void Shoot();

    StakeActor( const std::string& name );
    virtual ~StakeActor();

    StakeActor() = delete;
    StakeActor( const StakeActor& other ) = delete;
    StakeActor( StakeActor&& source ) = delete;
    StakeActor& operator=( const StakeActor& other ) = delete;
    StakeActor& operator=( StakeActor&& source ) = delete;
    bool operator==( const StakeActor& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:

};

} // end of namespace 'hunjam'
