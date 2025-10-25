#pragma once

#include "engine/actor/Actor.h"
#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( Pawn )

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
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
private:


    // Actor interface

};

