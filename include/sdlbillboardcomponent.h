/** -----------------------------------------------------------------------------
  * FILE:    sdlbillboardcomponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component for displaying billboards in SDL.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "sdldrawablecomponent.h"

namespace engine{


class SDLBillboardComponent: public SDLDrawableComponent
{
    enum class Mode{
        FilledRGBA,
        FramedFilledRGBA,
        //Procedural,       //TODO
        //FramedProcedural, //TODO
        //Textured,         //TODO
        //FramedTextured    //TODO
    };

public:
    SDLBillboardComponent(const std::string& name);
    SDLBillboardComponent(const SDLBillboardComponent& other);
    SDLBillboardComponent(SDLBillboardComponent&& other) = delete;
    virtual ~SDLBillboardComponent();
    SDLBillboardComponent& operator=(const SDLBillboardComponent &other) = delete;
    SDLBillboardComponent& operator=(SDLBillboardComponent &&other) = delete;
    bool operator==(const SDLBillboardComponent &other);

    virtual void tick(float t, float dt) override;
    virtual void OnRegistered() override;
    virtual void OnUnregistered() override;

    virtual std::vector<pt::math::float3> GetVertices() override;

    virtual void SetMode(Mode mode);
    virtual void SetHeight(float height);
    virtual void SetWidth(float width);

    virtual void SetBaseColor(const pt::math::float3& color);
    virtual void SetBaseColorAlpha(float alpha);
    virtual void SetFrameColor(const pt::math::float3& color);
    virtual void SetFrameColorAlpha(float alpha);


    Mode    GetMode() const;
    float   GetHeight() const;
    float   GetWidth() const;

    pt::math::float3    GetBaseColor() const;
    float               GetBaseColorAlpha() const;
    pt::math::float3    GetFrameColor() const;
    float               GetFrameColorAlpha() const;

    virtual bool    IsVisible() const;




protected:
    virtual void OnCreateContext() override;
    virtual void OnDestroyContext() override;

    virtual void Draw(float t, float dt) override;

    virtual void OnSpawn() override;
    virtual void OnDespawn() override;

private:
    float   mWidth;
    float   mHeight;

    Mode    mMode;

    pt::math::float3    mColorBase;
    float               mColorBaseAlpha;
    pt::math::float3    mColorFrame;
    float               mColorFrameAlpha;
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO

};


}

