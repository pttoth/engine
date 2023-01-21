#pragma once

#include "engine/experimental/Actor.h"

#include <cstdint>

class Scheduler
{
public:
    Scheduler();
    Scheduler( const Scheduler& other );
    Scheduler( Scheduler&& source );

    virtual ~Scheduler();

    Scheduler& operator=( const Scheduler& other );
    Scheduler& operator=( Scheduler&& source );

    bool operator==( const Scheduler& other ) const;


    void AddActor();
    void RemoveActor();
    void TickAllActors();

protected:

    struct TickUnit{
        Actor*      actor;
        uint64_t    lastTick;
        float       interval;
    };

private:
};


