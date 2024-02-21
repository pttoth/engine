/** -----------------------------------------------------------------------------
  * FILE:    PositionComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component representing a position (a transform).
  *          (by other definition: an instantiable WorldComponent)
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/WorldComponent.h"

#include <memory>

namespace engine{

class PositionComponent;
using PositionComponentPtr       = std::shared_ptr< PositionComponent >;
using ConstPositionComponentPtr  = std::shared_ptr< const PositionComponent >;
using PositionComponentWPtr      = std::weak_ptr< PositionComponent >;
using ConstPositionComponentWPtr = std::weak_ptr< const PositionComponent >;
using PositionComponentUPtr      = std::unique_ptr< PositionComponent >;
using ConstPositionComponentUPtr = std::unique_ptr< const PositionComponent >;

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

} //end of namespace 'engine'
