#pragma once

#include "engine/experimental/WorldComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class PositionComponent;
using PositionComponentPtr  = std::shared_ptr< PositionComponent >;
using PositionComponentPtrW = std::weak_ptr< PositionComponent >;

class PositionComponent: public WorldComponent
{
public:
    PositionComponent( const std::string& name );
    PositionComponent( const PositionComponent& other ) = delete;
    PositionComponent( PositionComponent&& source ) = delete;

    virtual ~PositionComponent();

    PositionComponent& operator=( const PositionComponent& other ) = delete;
    PositionComponent& operator=( PositionComponent&& source ) = delete;

    bool operator==( const PositionComponent& other ) const = delete;

    virtual void Tick( float t, float dt ) override;

protected:
    virtual void OnSpawned() override;
    virtual void OnDespawned() override;

private:
};

} //end of namespace experimental
} //end of namespace engine
