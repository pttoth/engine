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


    engine::SDLBillboardComponent* getBBC();

protected:
    virtual void tick(float t, float dt) override;

private:

    engine::SDLBillboardComponent mBillboardComponent;
    engine::SDLBillboardComponent mBbc;

};

} } //end of namespaces
