#pragma once

#include "engine/Actor.h"

#include <memory>

class BillboardActor;
using BillboardActorPtr       = std::shared_ptr< BillboardActor >;
using ConstBillboardActorPtr  = std::shared_ptr< const BillboardActor >;
using BillboardActorWPtr      = std::weak_ptr< BillboardActor >;
using ConstBillboardActorWPtr = std::weak_ptr< const BillboardActor >;
using BillboardActorUPtr      = std::unique_ptr< BillboardActor >;
using ConstBillboardActorUPtr = std::unique_ptr< const BillboardActor >;


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

protected:
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;

private:
    //members:
    //  texture asset path :String

};

