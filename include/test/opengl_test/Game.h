#pragma once

#include "engine/Engine.h"

#include "BillboardActor.h"
#include "Pawn.h"

#include <memory>

class Game;
using GamePtr       = std::shared_ptr< Game >;
using ConstGamePtr  = std::shared_ptr< const Game >;
using GameWPtr      = std::weak_ptr< Game >;
using ConstGameWPtr = std::weak_ptr< const Game >;
using GameUPtr      = std::unique_ptr< Game >;
using ConstGameUPtr = std::unique_ptr< const Game >;


class Game: public engine::Engine
{
public:
    Game() = delete;
    Game( int const argc, char* argv[] );
    Game( const Game& other ) = delete;
    Game( Game&& source ) = delete;
    virtual ~Game();
    Game& operator=( const Game& other ) = delete;
    Game& operator=( Game&& source ) = delete;
    bool operator==( const Game& other ) const = delete;
protected:
    void OnStart() override;
    void OnExit() override;
    void UpdateGameState( float t, float dt ) override;
private:

    //Pawn            mPlayerPawn;
    //BillboardActor  mBillboard;



};
