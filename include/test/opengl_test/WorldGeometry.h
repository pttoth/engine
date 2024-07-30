#pragma once

#include "engine/actor/Actor.h"
#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( WorldGeometry )

class WorldGeometry: public engine::Actor
{
public:
    WorldGeometry( const std::string& name );

    WorldGeometry( const WorldGeometry& other ) = delete;
    WorldGeometry( WorldGeometry&& source ) = delete;
    virtual ~WorldGeometry();
    WorldGeometry& operator=( const WorldGeometry& other ) = delete;
    WorldGeometry& operator=( WorldGeometry&& source ) = delete;

    bool operator==( const WorldGeometry& other ) const = delete;

protected:

    // @TODO: delet dis
    void TickComponents_NoDelay( float t, float dt ) override;
    void OnPreTickComponents( float t, float dt ) override;
    void OnPostTickComponents( float t, float dt ) override;



    void OnTick(float t, float dt) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    std::vector<engine::MeshComponentPtr> mMeshes;

};
