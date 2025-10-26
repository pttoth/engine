#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/MeshComponent.h"


#include "engine/Def.h"
#include "pt/macros.h"

namespace hunjam{

PT_FORWARD_DECLARE_CLASS( StaticActor )


class StaticActor: public engine::Actor
{
public:
    engine::AxisDisplayComponentPtr mAxis;
    engine::MeshComponentPtr        mMesh;


    StaticActor( const std::string& name );
    virtual ~StaticActor();

    StaticActor() = delete;
    StaticActor( const StaticActor& other ) = delete;
    StaticActor( StaticActor&& source ) = delete;
    StaticActor& operator=( const StaticActor& other ) = delete;
    StaticActor& operator=( StaticActor&& source ) = delete;
    bool operator==( const StaticActor& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:

};

} // end of namespace 'hunjam'
