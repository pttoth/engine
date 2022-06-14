#include "SerialScheduler.h"

#include "pt/utility.hpp"


#include "assert.h"

using namespace pt;
using namespace engine;


SerialScheduler::SerialScheduler():
    mPendingTasksTrigger(), mPendingTasks(mPendingTasksTrigger)
{

}

SerialScheduler::~SerialScheduler()
{

}


void SerialScheduler::
RegisterTick(Ticker& subject)
{
    Ticker* ps = &subject; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id( ps );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( ps->GetTickGroup() );

        //check if subject is already present
        int idx = pt::IndexOfInVector( vec_tickgroup, id );
        assert( idx < 0 );

        vec_tickgroup.push_back( id );

    }, EventExecRule::TriggerOnce );
}


void SerialScheduler::
UnregisterTick(Ticker &subject)
{
    Ticker* ps = &subject; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id( ps );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( ps->GetTickGroup() );

        //check if subject is missing
        int idx = pt::IndexOfInVector( vec_tickgroup, id );
        assert( -1 < idx );

        pt::RemoveElementInVector( vec_tickgroup, idx );

    }, EventExecRule::TriggerOnce );
}


void SerialScheduler::
AddTickDependency(Ticker& subject, Ticker& dependency)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer
    Ticker* pdep = &dependency;

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        TickDependencyData id_dependency( pdep );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure, that dependency is in the same tick group
            //this may happen during runtime, so
            //TODO: report ERROR with notification service (when it's done)
        assert( 0 <= pt::IndexOfInVector( vec_tickgroup, id_dependency ) );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //check if it already holds the dependency
        int idx_dep = pt::IndexOfInVector( vec_tickgroup[idx].dependencies, pdep );
        if( idx_dep < 0 ){ //if dependency is not contained yet
            vec_tickgroup[idx].dependencies.push_back( pdep );
        }

    }, EventExecRule::TriggerOnce );
}


void SerialScheduler::
RemoveTickDependency(Ticker& subject, Ticker& dependency)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer
    Ticker* pdep = &dependency;

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //check if it contains the dependency
        int idx_dep = pt::IndexOfInVector( vec_tickgroup[idx].dependencies, pdep );
        if( -1 < idx_dep ){
            pt::RemoveElementInVector( vec_tickgroup[idx].dependencies, idx_dep );
        }

    }, EventExecRule::TriggerOnce );
}


void SerialScheduler::
RemoveEntityDependencies(Ticker& subject)
{
    Ticker* psub = &subject;    //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        TickDependencyData id_subject( psub );
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( psub->GetTickGroup() );

        //make sure subject is present in the group
        int idx = pt::IndexOfInVector( vec_tickgroup, id_subject );
        assert( -1 < idx );

        //remove all dependencies
        vec_tickgroup[idx].dependencies.clear();

    }, EventExecRule::TriggerOnce );
}


void SerialScheduler::
RemoveDependenciesReferencingEntity(Ticker& dependency)
{
    Ticker* pdep = &dependency; //have the lambda capture a pointer

    mPendingTasks.addCallback( [=] () -> void{
        std::vector<TickDependencyData>& vec_tickgroup = GetTickGroupContainer( pdep->GetTickGroup() );

        //for each ticker
        for( auto tdd : vec_tickgroup ){
            //check if it depends on 'dependency'
            int idx_dep = pt::IndexOfInVector( tdd.dependencies, pdep );
            if( -1 < idx_dep ){
                //remove dependency
                pt::RemoveElementInVector( tdd.dependencies, idx_dep );
            }
        }

    }, EventExecRule::TriggerOnce );
}



void SerialScheduler::
Update(float t, float dt)
{
    TickPrePhysics( t, dt );
    //2 threads needed here
    //  t1:
    TickDuringPhysics( t, dt );
    //  t2:
//TODO: world.updatePhysics();

    TickPostPhysics( t, dt );
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
ProcessPendingTasks()
{
    mPendingTasksTrigger();
    mPendingTasks.clear();
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
