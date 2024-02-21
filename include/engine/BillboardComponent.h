#pragma once

#include "engine/RealComponent.h"

#include <memory>

namespace engine{

class BillboardComponent;
using BillboardComponentPtr       = std::shared_ptr< BillboardComponent >;
using ConstBillboardComponentPtr  = std::shared_ptr< const BillboardComponent >;
using BillboardComponentWPtr      = std::weak_ptr< BillboardComponent >;
using ConstBillboardComponentWPtr = std::weak_ptr< const BillboardComponent >;
using BillboardComponentUPtr      = std::unique_ptr< BillboardComponent >;
using ConstBillboardComponentUPtr = std::unique_ptr< const BillboardComponent >;


class BillboardComponent: public RealComponent
{
public:
    BillboardComponent( const std::string& name );
    BillboardComponent( const BillboardComponent& other ) = delete;
    BillboardComponent( BillboardComponent&& source ) = delete;
    virtual ~BillboardComponent(){}
    BillboardComponent& operator=( const BillboardComponent& other ) = delete;
    BillboardComponent& operator=( BillboardComponent&& source ) = delete;

    bool operator==( const BillboardComponent& other ) const = delete;

    void Draw( float t, float dt ) override;

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    void OnTick( float t, float dt ) override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:

};

} // end of namespace 'engine'
