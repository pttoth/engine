#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

//namespace engine{

PT_FORWARD_DECLARE_CLASS( StakeActor )

class StakeActor: public engine::Actor
{
public:
    engine::MeshComponent   mMesh;



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

//} // end of namespace 'engine'
