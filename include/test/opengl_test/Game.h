#pragma once

#include "engine/Engine.h"
#include "engine/SystemManager.h"
#include "pt/macros.h"
#include "BillboardActor.h"
#include "Pawn.h"

PT_FORWARD_DECLARE_CLASS( Game )

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
    Pawn            mPlayerPawn;
    //BillboardActor  mBillboard;




};
