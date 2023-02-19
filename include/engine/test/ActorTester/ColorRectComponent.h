#pragma once

#include "engine/experimental/SDLBillboardComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class ColorRectComponent;
using ColorRectComponentPtr  = std::shared_ptr< ColorRectComponent >;
using ColorRectComponentPtrW = std::weak_ptr< ColorRectComponent >;

class ColorRectComponent: public SDLBillboardComponent
{
public:
    ColorRectComponent( const std::string& name );
    ColorRectComponent( const ColorRectComponent& other );
    ColorRectComponent( ColorRectComponent&& source );

    virtual ~ColorRectComponent();

    ColorRectComponent& operator=( const ColorRectComponent& other );
    ColorRectComponent& operator=( ColorRectComponent&& source );

    bool operator==( const ColorRectComponent& other ) const;

    void Tick(float t, float dt) override;

    void Draw(float t, float dt) override;

    void SetNextColor();
    pt::math::float3 GetColor() const;

    void SetID(int32_t id);
    int32_t GetID() const;

    static void ResetLastID();


protected:
    void OnSpawned() override;
    void OnDespawned() override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    void init();

    uint32_t mIdxColor = 0;
    pt::math::float3 mColor = pt::math::float3::white;

    float   mLastUpdateT = 0.0f;
    int32_t mID = 0;



    static std::vector<pt::math::float3> CRCColors;
    static int32_t  LastID;
    static bool     Locked;

};

} //end of namespace experimental
} //end of namespace engine
