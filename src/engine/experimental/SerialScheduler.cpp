#include "engine/experimental/SerialScheduler.h"

#include "pt/event.hpp"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

using namespace pt;

SerialScheduler::
SerialScheduler():
    mPendingTasks( mPendingTasksTrigger )
{}


SerialScheduler::
SerialScheduler( const SerialScheduler& other ):
    mPendingTasks( mPendingTasksTrigger )
{}


SerialScheduler::
SerialScheduler( SerialScheduler&& source ):
    mPendingTasks( mPendingTasksTrigger )
{}


SerialScheduler::
~SerialScheduler()
{}


SerialScheduler &SerialScheduler::
operator=( const SerialScheduler& other )
{
    assert( false );
}


SerialScheduler &SerialScheduler::
operator=( SerialScheduler&& source )
{
    assert( false );
}


bool SerialScheduler::
operator==( const SerialScheduler& other ) const
{
    assert( false );
}


void SerialScheduler::
AddActor( Actor& subject )
{
    Actor* ps = &subject; //have the lambda capture a pointer

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
RemoveActor( Actor& subject )
{
    Actor* ps = &subject; //have the lambda capture a pointer

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
AddTickDependency( Actor& subject, Actor& dependency )
{
    Actor* psub = &subject;    //have the lambda capture a pointer
    Actor* pdep = &dependency;

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
RemoveTickDependency( Actor& subject, Actor& dependency )
{
    Actor* psub = &subject;    //have the lambda capture a pointer
    Actor* pdep = &dependency;

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
RemoveDependenciesForActor( Actor& subject )
{
    Actor* psub = &subject;    //have the lambda capture a pointer

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
RemoveDependenciesReferencingActor( Actor& dependency )
{
    Actor* pdep = &dependency; //have the lambda capture a pointer

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
TickAllActors()
{

}


std::vector<SerialScheduler::TickDependencyData>& SerialScheduler::
GetTickGroupContainer( TickGroup tg )
{
    switch ( tg ) {
    case TickGroup::DURINGPHYSICS:
        return mTickDepDuringphysics;
    case TickGroup::PREPHYSICS:
        return mTickDepPrephysics;
    case TickGroup::POSTPHYSICS:
        return mTickDepPostphysics;
    case TickGroup::NO_GROUP:
        assert( false );
        throw std::invalid_argument( "Cannot return TickGroup container for TickGroup::NO_GROUP" );
    }
    throw std::invalid_argument( "invalid enum value" );
}

