#pragma once

#include "engine/experimental/Actor.h"
#include "engine/test/ActorTester/ColorRectComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class PlayerPawn;
using PlayerPawnPtr  = std::shared_ptr< PlayerPawn >;
using PlayerPawnPtrW = std::weak_ptr< PlayerPawn >;

class PlayerPawn: public Actor
{
public:
    PlayerPawn( const std::string& name );
    PlayerPawn( const PlayerPawn& other );
    PlayerPawn( PlayerPawn&& source );

    virtual ~PlayerPawn();

    PlayerPawn& operator=( const PlayerPawn& other );
    PlayerPawn& operator=( PlayerPawn&& source );

    bool operator==( const PlayerPawn& other ) const;

    void Tick();

protected:
    void TickComponents();
    void SetParent();
    void RemoveParent();

private:
    ColorRectComponent mColorRectComp;

};

} //end of namespace experimental
} //end of namespace engine
