#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( RotationTestActor )


class RotationTestActor: public engine::Actor
{
public:
    RotationTestActor( const std::string& name );
    virtual ~RotationTestActor();

    RotationTestActor() = delete;
    RotationTestActor( const RotationTestActor& other ) = delete;
    RotationTestActor( RotationTestActor&& source ) = delete;
    RotationTestActor& operator=( const RotationTestActor& other ) = delete;
    RotationTestActor& operator=( RotationTestActor&& source ) = delete;
    bool operator==( const RotationTestActor& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    engine::AxisDisplayComponentPtr mAxis;
    engine::MeshComponentPtr        mMesh;

};
