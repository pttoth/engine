#pragma once

#include "engine/SDLBillboardComponent.h"

#include <memory>

namespace engine{

class ColorRectComponent;
using ColorRectComponentPtr  = std::shared_ptr< ColorRectComponent >;
using ColorRectComponentPtrW = std::weak_ptr< ColorRectComponent >;

class ColorRectComponent: public SDLBillboardComponent
{
public:
    ColorRectComponent( const std::string& name );
    ColorRectComponent( const ColorRectComponent& other ) = delete;
    ColorRectComponent( ColorRectComponent&& source ) = delete;

    virtual ~ColorRectComponent();

    ColorRectComponent& operator=( const ColorRectComponent& other ) = delete;
    ColorRectComponent& operator=( ColorRectComponent&& source ) = delete;

    bool operator==( const ColorRectComponent& other ) const = delete;

    void Draw(float t, float dt) override;

    void SetNextColor();
    math::float3 GetColor() const;

    void SetID(int32_t id);
    int32_t GetID() const;

    static void ResetLastID();


protected:
    void OnSpawned() override;
    void OnDespawned() override;
    void OnTick( float t, float dt ) override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    void init();

    uint32_t mIdxColor = 0;
    math::float3 mColor = math::float3::white;

    float   mLastUpdateT = 0.0f;
    int32_t mID = 0;



    static std::vector<math::float3> CRCColors;
    static int32_t  LastID;
    static bool     Locked;

};

} //end of namespace engine
