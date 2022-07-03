#pragma once

#include "engine/Entity.h"

#include "engine/test/ticktester/ColorRectComponent.h"

namespace test{
namespace ticktester{

class PlayerPawn: public engine::Entity,
                  public IListener
{

public:
    PlayerPawn(const std::string& name);
    PlayerPawn(const PlayerPawn& other);
    PlayerPawn(PlayerPawn&& other) = delete;
    virtual ~PlayerPawn();
    PlayerPawn& operator=(const PlayerPawn &other) = delete;
    PlayerPawn& operator=(PlayerPawn &&other) = delete;
    bool operator==(const PlayerPawn &other) const;

    virtual void OnSpawned() override;
    virtual void OnDespawned() override;

    void SetFloatRadius(float r);
    float GetFloatRadius() const;


    void ShuffleTickOrder();

    ColorRectComponent* getMainRect();
    ColorRectComponent* getSubRect();
    ColorRectComponent* getFloatingRect();

// IListener interface
    virtual void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    virtual void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    virtual void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    virtual void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

protected:
    virtual void Tick(float t, float dt) override;

private:

    ColorRectComponent mMainRect;
    ColorRectComponent mSubRect;
    ColorRectComponent mFloatingRect;
    float mLastFloatingTime = 0.0f;
    float mFloatRadius = 0.4f;

    std::vector<ColorRectComponent*> mRectangles;


};

} } //end of namespaces
