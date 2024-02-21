#pragma once

#include "engine/WorldComponent.h"

#include <memory>

namespace engine{

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

protected:
    void OnSpawned() override;
    void OnDespawned() override;
    void OnTick( float t, float dt ) override;

private:

};

} //end of namespace engine
