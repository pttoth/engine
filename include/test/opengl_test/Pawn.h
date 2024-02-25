#pragma once

#include "engine/Actor.h"

#include <memory>

class Pawn;
using PawnPtr       = std::shared_ptr< Pawn >;
using ConstPawnPtr  = std::shared_ptr< const Pawn >;
using PawnWPtr      = std::weak_ptr< Pawn >;
using ConstPawnWPtr = std::weak_ptr< const Pawn >;
using PawnUPtr      = std::unique_ptr< Pawn >;
using ConstPawnUPtr = std::unique_ptr< const Pawn >;


class Pawn: engine::Actor
{
public:
    Pawn( const std::string& name );
    Pawn( const Pawn& other ) = delete;
    Pawn( Pawn&& source ) = delete;
    virtual ~Pawn();
    Pawn& operator=( const Pawn& other ) = delete;
    Pawn& operator=( Pawn&& source ) = delete;
    bool operator==( const Pawn& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
private:

};

