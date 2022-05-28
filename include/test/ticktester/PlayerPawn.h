#pragma once

#include "Entity.h"

#include "ColorRectComponent.h"

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

    void SetFloatRadius(float r);
    float GetFloatRadius() const;

    ColorRectComponent* getMainRect();
    ColorRectComponent* getSubRect();
    ColorRectComponent* getFloatingRect();

protected:
    virtual void tick(float t, float dt) override;

private:

    ColorRectComponent mMainRect;
    ColorRectComponent mSubRect;
    ColorRectComponent mFloatingRect;
    float mLastFloatingTime;
    float mFloatRadius = 0.4f;

};

} } //end of namespaces
