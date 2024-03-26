/** -----------------------------------------------------------------------------
  * FILE:    PositionComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: A component representing a position (a transform).
  *          (by other definition: an instantiable WorldComponent)
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "WorldComponent.h"
#include "pt/macros.h"
#include <memory>

namespace engine{

PT_FORWARD_DECLARE_CLASS( PositionComponent )

class PositionComponent: public WorldComponent
{
public:
    PositionComponent( const std::string& name );
    PositionComponent( const PositionComponent& other ) = delete;
    PositionComponent( PositionComponent&& source ) = delete; // TODO: enable
    virtual ~PositionComponent();
    PositionComponent& operator=( const PositionComponent& other ) = delete;
    PositionComponent& operator=( PositionComponent&& source ) = delete; // TODO: enable

    bool operator==( const PositionComponent& other ) const = delete;

protected:
    void OnSpawned() override;
    void OnDespawned() override;
    void OnTick( float t, float dt ) override;

private:

};

} //end of namespace 'engine'
