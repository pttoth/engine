#pragma once

#include "engine/Actor.h"
#include "engine/test/ActorTester/ColorRectComponent.h"

#include <memory>

namespace engine{

class PlayerPawn;
using PlayerPawnPtr  = std::shared_ptr< PlayerPawn >;
using PlayerPawnPtrW = std::weak_ptr< PlayerPawn >;

class PlayerPawn: public Actor
{
public:
    PlayerPawn( const std::string& name );
    PlayerPawn( const PlayerPawn& other ) = delete;
    PlayerPawn( PlayerPawn&& source ) = delete;

    virtual ~PlayerPawn();

    PlayerPawn& operator=( const PlayerPawn& other ) = delete;
    PlayerPawn& operator=( PlayerPawn&& source ) = delete;

    bool operator==( const PlayerPawn& other ) const = delete;

    void init();

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    void OnTick( float t, float dt ) override;

private:
    ColorRectComponent mColorRectComp;

};

} //end of namespace engine
