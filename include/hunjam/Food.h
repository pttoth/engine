#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"


namespace hunjam{

PT_FORWARD_DECLARE_CLASS( Food )


class Food: public engine::Actor
{
public:
    struct Launcher{
        math::vec3 mPos;
        math::mat4 mTransform = math::mat4::identity;

        Launcher( math::vec3 position, math::FRotator dir ):
            mPos( position )
        {
            math::mat4 t = math::mat4::translation( position );
            mTransform =  t * dir.GetTransform();
        }
    };

    engine::AxisDisplayComponentPtr mAxis;
    engine::MeshComponentPtr        mMesh;
    engine::MeshComponentPtr        mMeshCollider;


    std::vector<Launcher>     mLaunchers;

    bool    mIsFlying       = false;
    bool    mHasLanded      = false;

    float               mThrowForce     = 2500;
    math::vec3          mInertia;
    const math::vec3    mGravity        = math::vec3( 0, 0, -9.82f ) * 100;

    uint64_t mLandTime = 0;


    float       mLastTriggerTime;

    void        Launch();



    Food( const std::string& name );
    virtual ~Food();

    Food() = delete;
    Food( const Food& other ) = delete;
    Food( Food&& source ) = delete;
    Food& operator=( const Food& other ) = delete;
    Food& operator=( Food&& source ) = delete;
    bool operator==( const Food& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:

};

} // end of namespace 'hunjam'
