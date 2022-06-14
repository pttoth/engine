#pragma once

#include "engine/SDLBillboardComponent.h"
#include "engine/test/ticktester/IListener.h"

#include <vector>
#include <string>


class ColorRectComponent: public engine::SDLBillboardComponent,
                          public IListener
{

public:
    ColorRectComponent(const std::string& name, int32_t id);
    ColorRectComponent(const ColorRectComponent& other);
    ColorRectComponent(ColorRectComponent&& other) = delete;
    virtual ~ColorRectComponent();
    ColorRectComponent& operator=(const ColorRectComponent &other) = delete;
    ColorRectComponent& operator=(ColorRectComponent &&other) = delete;
    bool operator==(const ColorRectComponent &other)const;


    void tick(float t, float dt);

    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

    void SetNextColor();
    pt::math::float3 GetColor() const;

    void SetID(int32_t id);
    int32_t GetID() const;

    static void ResetLastID();

protected:
    void onSpawn();
private:
    uint32_t mIdxColor = 0;
    pt::math::float3 mColor = pt::math::float3::white;

    float   mLastUpdateT = 0.0f;
    int32_t mID = 0;



    static std::vector<pt::math::float3> CRCColors;
    static int32_t  LastID;
    static bool     Locked;



};

