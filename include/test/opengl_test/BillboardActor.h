#pragma once

#include "engine/Actor.h"

#include "test/opengl_test/AxisDisplayComponent.h"

#include "engine/BillboardComponent.h"
#include "engine/gl/Texture.h"
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
    engine::AxisDisplayComponentPtr mAxis;

};
