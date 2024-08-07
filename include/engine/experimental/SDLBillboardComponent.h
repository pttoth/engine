#pragma once

#include "engine/experimental/SDLComponent.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( SDLBillboardComponent )

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
    SDLBillboardComponent( SDLBillboardComponent&& source ) = delete; // TODO: enable

    virtual ~SDLBillboardComponent();

    SDLBillboardComponent& operator=( const SDLBillboardComponent& other ) = delete;
    SDLBillboardComponent& operator=( SDLBillboardComponent&& source ) = delete; // TODO: enable

    bool operator==( const SDLBillboardComponent& other ) const = delete;

    virtual std::vector<math::float3> GetVertices();

    virtual void SetFrameEnabled(bool enabled);

    virtual void SetMode(Mode mode);
    virtual void SetHeight(float height);
    virtual void SetWidth(float width);

    virtual void SetBaseColorF(float red, float green, float blue, float alpha = 1.0f);
    virtual void SetBaseColor(const math::float3& color, float alpha = 1.0f);
    virtual void SetBaseColorAlpha(float alpha);
    virtual void SetFrameColorF(float red, float green, float blue, float alpha = 1.0f);
    virtual void SetFrameColor(const math::float3& color, float alpha = 1.0f);
    virtual void SetFrameColorAlpha(float alpha);

    bool    GetFrameEnabled() const;

    Mode    GetMode() const;
    float   GetHeight() const;
    float   GetWidth() const;

    math::float3    GetBaseColorF3() const;
    math::float4    GetBaseColorF4() const;
    float           GetBaseColorAlpha() const;
    math::float3    GetFrameColorF3() const;
    math::float4    GetFrameColorF4() const;
    float           GetFrameColorAlpha() const;

    virtual bool    IsVisible() const;

    void Draw(float t, float dt) override;

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    //void OnTick( float t, float dt ) override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;
private:
    float   mWidth;
    float   mHeight;

    Mode    mMode;
    bool    mHasFrame;

    math::float4    mColorBase;
    math::float4    mColorFrame;
    //type...         mProcedural;      //TODO
    //Texture         mTexture;         //TODO


};

} //end of namespace 'engine'
