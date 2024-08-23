#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/MeshComponent.h"

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

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    engine::AxisDisplayComponentPtr mAxis;
    engine::MeshComponentPtr mMesh;

};
