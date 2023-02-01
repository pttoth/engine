#pragma once

#include "engine/experimental/WorldComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class RealComponent;
using RealComponentPtr  = std::shared_ptr< RealComponent >;
using RealComponentPtrW = std::weak_ptr< RealComponent >;

class RealComponent: public WorldComponent
{
public:
    RealComponent( const std::string& name );
    RealComponent( const RealComponent& other );
    RealComponent( RealComponent&& source );

    virtual ~RealComponent();

    RealComponent& operator=( const RealComponent& other ) = delete;
    RealComponent& operator=( RealComponent&& source ) = delete;

    bool operator==( const RealComponent& other ) const = delete;


    void OnAddedToEntity( ComponentVisitor& visitor ) override;
    void OnRemovedFromEntity( ComponentVisitor& visitor ) override;

    void Spawn() override;
    void Despawn() override;

    void Tick( float t, float dt ) override;


protected:
private:
};

} //end of namespace experimental
} //end of namespace engine
