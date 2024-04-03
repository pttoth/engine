#pragma once

#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( Game )


class Game
{
public:
    Game();
    //Game( const std::string& name );

    Game( const Game& other );
    Game( Game&& source );
    virtual ~Game();
    Game& operator=( const Game& other );
    Game& operator=( Game&& source );

    bool operator==( const Game& other ) const;
protected:
private:
};

//} // end of namespace 'ClassNameSpace'
