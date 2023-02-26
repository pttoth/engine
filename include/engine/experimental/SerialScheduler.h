#pragma once

#include "engine/experimental/Scheduler.h"

#include <memory>

namespace engine{
namespace experimental{

class SerialScheduler;
using SerialSchedulerPtr  = std::shared_ptr< SerialScheduler >;
using SerialSchedulerPtrW = std::weak_ptr< SerialScheduler >;

class SerialScheduler: public Scheduler
{
public:
    SerialScheduler();
    SerialScheduler( const SerialScheduler& other ) = delete;
    SerialScheduler( SerialScheduler&& source ) = delete;
    virtual ~SerialScheduler();
    SerialScheduler& operator=( const SerialScheduler& other ) = delete;
    SerialScheduler& operator=( SerialScheduler&& source ) = delete;
    bool operator==( const SerialScheduler& other ) const = delete;


    void AddActor( Actor& subject, TickGroup tickgroup ) override;
    void RemoveActor( Actor& subject ) override;
    void AddTickDependency( Actor& subject, Actor& dependency ) override;
    void RemoveTickDependency( Actor& subject, Actor& dependency ) override;
    void RemoveDependenciesForActor( Actor& subject ) override;
    void RemoveDependenciesReferencingActor( Actor& dependency ) override;

    void ClearUnusedTickData();
    void ProcessPendingTasks();

    void TickPrePhysics( float t, float dt );
    void TickDuringPhysics( float t, float dt );
    void TickPostPhysics( float t, float dt );

    void TickAllActors();

protected:
private:
    struct TickDependencyData;
    struct TickDependencyData{
        Actor* subject;
        std::vector<Actor*> dependencies;
        bool active;
        bool ticked;
        TickDependencyData( Actor* e, bool a = true ):
            subject(e), active(a),
            ticked(false)
        {}
        TickDependencyData( const TickDependencyData& other ) = default;
        TickDependencyData( TickDependencyData&& source ) = default;
        TickDependencyData& operator=( const TickDependencyData& other )= default;
        TickDependencyData& operator=( TickDependencyData&& source ) = default;
        //only checks if subject Entity is the same
        bool operator==( const TickDependencyData& other ) const{
            return ( subject == other.subject );
        }
        //only checks if subject Entity is the same
        bool operator==( const Actor* e ) const{
            return ( subject == e );
        }
        //tells, whether the Entity should tick this frame
        bool shouldTick(){
            return ( active && !ticked );
        }
    };

    uint32_t    mUptime;
    uint32_t    mTickrate;

    pt::EventTrigger<> mPendingTasksTrigger;
    pt::Event<>        mPendingTasks;


    void TickElementsInGroupContainer( std::vector<TickDependencyData>& container, float t, float dt );
    std::vector<TickDependencyData>& GetTickGroupContainer( TickGroup tg );

    std::vector<TickDependencyData> mTickDepPrephysics;
    std::vector<TickDependencyData> mTickDepDuringphysics;
    std::vector<TickDependencyData> mTickDepPostphysics;

};

} //end of namespace experimental
} //end of namespace engine
