#pragma once

#include "engine/Entity.h"

#include "engine/test/ticktester/ColorRectComponent.h"

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

    void SetFloatRadius(float r);
    float GetFloatRadius() const;

    ColorRectComponent* getMainRect();
    ColorRectComponent* getSubRect();
    ColorRectComponent* getFloatingRect();

protected:
    virtual void Tick(float t, float dt) override;

private:

    ColorRectComponent mMainRect;
    ColorRectComponent mSubRect;
    ColorRectComponent mFloatingRect;
    float mLastFloatingTime;
    float mFloatRadius = 0.4f;

};

} } //end of namespaces
