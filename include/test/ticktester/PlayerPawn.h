#pragma once

#include "entity.h"

#include "sdlbillboardcomponent.h"

namespace pt{
namespace ticktester{

class PlayerPawn: public engine::Entity
{

public:
    PlayerPawn();
    PlayerPawn(const PlayerPawn& other) = delete;
    PlayerPawn(PlayerPawn&& other) = delete;
    virtual ~PlayerPawn();
    PlayerPawn& operator=(const PlayerPawn &other) = delete;
    PlayerPawn& operator=(PlayerPawn &&other) = delete;
    bool operator==(const PlayerPawn &other) const;

    virtual void OnRegister() override;
    virtual void OnUnregister() override;
    virtual void OnCreateContext() override;
    virtual void OnDeleteContext() override;

protected:
    virtual void tick(float t, float dt) override;

private:

    engine::SDLBillboardComponent mBillboardComponent;


};

} } //end of namespaces
