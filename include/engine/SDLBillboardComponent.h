#pragma once

#include "engine/SDLComponent.h"

#include <memory>

namespace engine{

class SDLBillboardComponent;
using SDLBillboardComponentPtr  = std::shared_ptr< SDLBillboardComponent >;
using SDLBillboardComponentPtrW = std::weak_ptr< SDLBillboardComponent >;

class SDLBillboardComponent: public SDLComponent
{
public:
    enum class Mode{
        FramedRGBA = 0,
        FilledRGBA,
        Procedural,       //TODO
        Textured,         //TODO
    };


    SDLBillboardComponent( const std::string& name );
    SDLBillboardComponent( const SDLBillboardComponent& other ) = delete;
    SDLBillboardComponent( SDLBillboardComponent&& source ) = delete;

    virtual ~SDLBillboardComponent();

    SDLBillboardComponent& operator=( const SDLBillboardComponent& other ) = delete;
    SDLBillboardComponent& operator=( SDLBillboardComponent&& source ) = delete;

    bool operator==( const SDLBillboardComponent& other ) const = delete;

    virtual void Tick(float t, float dt) override;

    virtual std::vector<pt::math::float3> GetVertices();

    virtual void SetFrameEnabled(bool enabled);

    virtual void SetMode(Mode mode);
    virtual void SetHeight(float height);
    virtual void SetWidth(float width);

    virtual void SetBaseColorF(float red, float green, float blue, float alpha = 1.0f);
    virtual void SetBaseColor(const pt::math::float3& color, float alpha = 1.0f);
    virtual void SetBaseColorAlpha(float alpha);
    virtual void SetFrameColorF(float red, float green, float blue, float alpha = 1.0f);
    virtual void SetFrameColor(const pt::math::float3& color, float alpha = 1.0f);
    virtual void SetFrameColorAlpha(float alpha);

    bool    GetFrameEnabled() const;

    Mode    GetMode() const;
    float   GetHeight() const;
    float   GetWidth() const;

    pt::math::float3    GetBaseColorF3() const;
    pt::math::float4    GetBaseColorF4() const;
    float               GetBaseColorAlpha() const;
    pt::math::float3    GetFrameColorF3() const;
    pt::math::float4    GetFrameColorF4() const;
    float               GetFrameColorAlpha() const;

    virtual bool    IsVisible() const;

    void Draw(float t, float dt) override;

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;
private:
    float   mWidth;
    float   mHeight;

    Mode    mMode;
    bool    mHasFrame;

    pt::math::float4    mColorBase;
    pt::math::float4    mColorFrame;
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO


};

} //end of namespace engine
