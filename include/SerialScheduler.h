#pragma once

#include "Scheduler.h"

namespace engine{

class SerialScheduler: public Scheduler
{

public:
    virtual void RegisterTick(Ticker& subject) override;
    virtual void UnregisterTick(Ticker& subject) override;
    virtual void AddTickDependency(Ticker& subject, Ticker& dependency) override;
    virtual void RemoveTickDependency(Ticker& subject, Ticker& dependency) override;

    virtual void RemoveEntityDependencies(Ticker& subject) override;
    virtual void RemoveDependenciesReferencingEntity(Ticker& dependency) override;

    virtual void Update() override;
};

}
