#pragma once

#include "Entity.h"

#include "SDLBillboardComponent.h"

namespace test{
namespace ticktester{

class PlayerPawn: public engine::Entity
{

public:
    PlayerPawn(const std::string& name);
    PlayerPawn(const PlayerPawn& other);
    PlayerPawn(PlayerPawn&& other) = delete;
    virtual ~PlayerPawn();
    PlayerPawn& operator=(const PlayerPawn &other) = delete;
    PlayerPawn& operator=(PlayerPawn &&other) = delete;
    bool operator==(const PlayerPawn &other) const;

    virtual void OnRegister() override;
    virtual void OnUnregister() override;


    engine::SDLBillboardComponent* getMainRect();
    engine::SDLBillboardComponent* getSubRect();
    engine::SDLBillboardComponent* getFloatingRect();

protected:
    virtual void tick(float t, float dt) override;

private:

    engine::SDLBillboardComponent mMainRect;
    engine::SDLBillboardComponent mSubRect;
    engine::SDLBillboardComponent mFloatingRect;
    float mLastFloatingTime;


};

} } //end of namespaces
