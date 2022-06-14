#pragma once

#include "Scheduler.h"

#include "pt/event.hpp"

#include <vector>
#include <cstdint>

namespace engine{

class Ticker;

class SerialScheduler: public Scheduler
{

public:
    SerialScheduler();
    virtual ~SerialScheduler();


    virtual void RegisterTick(Ticker& subject) override;
    virtual void UnregisterTick(Ticker& subject) override;
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) override;
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) override;

    virtual void RemoveEntityDependencies(Ticker& subject) override;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) override;

    virtual void Update();

private:
    struct TickDependencyData;
    struct TickDependencyData{
        Ticker* subject;
        std::vector<Ticker*> dependencies;
        bool active;
        bool ticked;
        TickDependencyData(Ticker* e, bool a = true):
            subject(e), active(a),
            ticked(false)
        {}
        TickDependencyData(const TickDependencyData& other) = default;
        TickDependencyData(TickDependencyData&& source) = default;
        TickDependencyData& operator=(const TickDependencyData&other)= default;
        TickDependencyData& operator=(TickDependencyData&& source) = default;
        //only checks if subject Entity is the same
        bool operator==(const TickDependencyData &other)const{
            return (subject == other.subject);
        }
        //only checks if subject Entity is the same
        bool operator==(const Ticker* e)const{
            return (subject == e);
        }
        //tells, whether the Entity should tick this frame
        bool shouldTick(){ return (active && !ticked); }
    };

    uint32_t    mUptime;
    uint32_t    mTickrate;

    pt::EventTrigger<> mPendingTasksTrigger;
    pt::Event<>        mPendingTasks;


    void TickElementsInGroupContainer(std::vector<TickDependencyData>& container, float t, float dt);
    std::vector<TickDependencyData>& GetTickGroupContainer(Ticker::Group tg);

    std::vector<TickDependencyData> mTickDepPrephysics;
    std::vector<TickDependencyData> mTickDepDuringphysics;
    std::vector<TickDependencyData> mTickDepPostphysics;


    void ClearUnusedTickData();
    void ProcessPendingTasks();

    void TickPrePhysics(float t, float dt);
    void TickDuringPhysics(float t, float dt);
    void TickPostPhysics(float t, float dt);

};

}
