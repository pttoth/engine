#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/BillboardComponent.h"

#include "engine/component/MeshComponent.h"

#include "engine/gl/Texture2d.h"
#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( BillboardActor )

class BillboardActor: public engine::Actor
{
public:
    BillboardActor() = delete;
    BillboardActor( const std::string& name );
    BillboardActor( const BillboardActor& other ) = delete;
    BillboardActor( BillboardActor&& source ) = delete;
    virtual ~BillboardActor();
    BillboardActor& operator=( const BillboardActor& other ) = delete;
    BillboardActor& operator=( BillboardActor&& source ) = delete;
    bool operator==( const BillboardActor& other ) const = delete;

    void SetTexture( engine::gl::Texture2dPtr texture );
    void SetMesh( engine::gl::MeshPtr mesh );
    void SetMesh( std::string& name );

protected:
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;

private:
    //members:
    //  texture asset path :String
    engine::BillboardComponentPtr   mBillboardComponent;
    engine::AxisDisplayComponentPtr mActorAxis;

    engine::MeshComponentPtr        mMeshComponent;

    engine::MeshComponentPtr        mCameraMeshComponent;
};
