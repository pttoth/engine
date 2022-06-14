#include "SerialScheduler.h"

#include "pt/utility.hpp"


#include "assert.h"

using namespace engine;


SerialScheduler::SerialScheduler():
    mPendingTasksTrigger(), mPendingTasks(mPendingTasksTrigger)
{

}

SerialScheduler::~SerialScheduler()
{

}


void SerialScheduler::
RegisterTick(Ticker &subject)
{

}


void SerialScheduler::
UnregisterTick(Ticker &subject)
{

}


void SerialScheduler::
AddTickDependency(Ticker &subject, Ticker &dependency)
{

}


void SerialScheduler::
RemoveTickDependency(Ticker &subject, Ticker &dependency)
{

}


void SerialScheduler::
RemoveEntityDependencies(Ticker &subject)
{

}


void SerialScheduler::
RemoveDependenciesReferencingEntity(Ticker &dependency)
{

}


void SerialScheduler::
Update()
{
    mPendingTasksTrigger();
    mPendingTasks.clear();
}

//-----


std::vector<SerialScheduler::TickDependencyData> &SerialScheduler::
GetTickGroupContainer(Ticker::Group tg)
{
    assert( tg != Ticker::Group::NO_GROUP);
    switch( tg ){
    case Ticker::Group::PREPHYSICS:     return mTickDepPrephysics;
    case Ticker::Group::DURINGPHYSICS:  return mTickDepDuringphysics;
    case Ticker::Group::POSTPHYSICS:    return mTickDepPostphysics;
    default:                            assert(false); //mem garbage value
    }
}








void SerialScheduler::
ClearUnusedTickData()
{
    Ticker::Group groups[] ={Ticker::Group::PREPHYSICS,
                             Ticker::Group::DURINGPHYSICS,
                             Ticker::Group::POSTPHYSICS };
    for(auto tg : groups){
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( tg );
        //iterate backwards (removal messes up right side of vector)
        for( int idx=vec_tickgroup.size(); 0<=idx; --idx ){
            //remove each inactive entry (starting from backwards)
            if( !vec_tickgroup[idx].active ){
                pt::RemoveElementInVector( vec_tickgroup, idx );
            }
        }
    }
}


void SerialScheduler::
TickElementsInGroupContainer(std::vector<TickDependencyData> &tg_container,
                             float t, float dt)
{
    size_t size = tg_container.size();
    size_t count = 0;
    size_t safety = 0;
    //don't tick empty container
    if( 0 == size ){
        return;
    }
    //reset container metadata
    for( TickDependencyData& d : tg_container ){
        d.ticked = false; //doesnt care about active/inactive check (no need and more costly to check)
    }
    while( count < size ){
        //ran 'size' times and couldn't tick everyone!
        assert( safety < size );

        for(TickDependencyData& tdd : tg_container){
            if( tdd.shouldTick() ){
                //resolve dependencies
                bool canGo = true;
                for( Ticker* dep : tdd.dependencies ){
                    //find dependency
                    TickDependencyData d(dep);
                    int idx_dep = pt::IndexOfInVector( tg_container , d);
                    if( !tg_container[idx_dep].ticked ){
                    //if dependency haven't ticked, skip us for now
                        canGo = false;
                    }
                }
                //if dependencies are done, tick entity
                //  otherwise wait for next pass
                if( canGo ){
                    tdd.subject->Tick( t, dt );
                    tdd.ticked = true;
                    ++count;
                }
            }
        }
        ++safety;
    }
}


void SerialScheduler::
TickPrePhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepPrephysics, t, dt );
}


void SerialScheduler::
TickDuringPhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepDuringphysics, t, dt );
}


void SerialScheduler::
TickPostPhysics(float t, float dt)
{
    TickElementsInGroupContainer( mTickDepPostphysics, t, dt );
}
