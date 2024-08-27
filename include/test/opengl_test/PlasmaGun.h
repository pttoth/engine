#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/MeshComponent.h"
#include "engine/component/LightPointComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( PlasmaGun )

class PlasmaGun: public engine::Actor
{
public:
    PlasmaGun( const std::string& name );
    virtual ~PlasmaGun();

    PlasmaGun() = delete;
    PlasmaGun( const PlasmaGun& other ) = delete;
    PlasmaGun( PlasmaGun&& source ) = delete;
    PlasmaGun& operator=( const PlasmaGun& other ) = delete;
    PlasmaGun& operator=( PlasmaGun&& source ) = delete;
    bool operator==( const PlasmaGun& other ) const = delete;

    void Shoot();

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

    math::vec3 GetProjectileSpawnLocation() const;
    int32_t FindFreeProjectileIndex() const;


private:
    engine::AxisDisplayComponentPtr mAxis;
    engine::MeshComponentPtr        mMesh;

    static const size_t     mMaxProjectileLifetime = 15; // seconds
    static const size_t     mProjectileSpeed = 25;
    static const int32_t    mMaxProjectileCount = 16;
    int32_t                 mSpawnedProjectiles = 0;

    engine::LightPointComponentPtr  mProjectileLights[mMaxProjectileCount];
    engine::MeshComponentPtr        mProjectileMeshes[mMaxProjectileCount];
    int64_t                         mProjectileSpawntime[mMaxProjectileCount];
};
