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
    SDLBillboardComponent();
    SDLBillboardComponent(const SDLBillboardComponent& other) = delete;
    SDLBillboardComponent(SDLBillboardComponent&& other) = delete;
    virtual ~SDLBillboardComponent();
    SDLBillboardComponent& operator=(const SDLBillboardComponent &other) = delete;
    SDLBillboardComponent& operator=(SDLBillboardComponent &&other) = delete;
    bool operator==(const SDLBillboardComponent &other);

public:
    virtual void tick(float t, float dt) override;
    virtual void OnRegistered() override;
    virtual void OnUnregistered() override;

    virtual void draw() override;

    virtual void SetMode(Mode mode);
    virtual void SetHeight(float height);
    virtual void SetWidth(float width);

    Mode    GetMode() const;
    float   GetHeight() const;
    float   GetWidth() const;

    virtual bool    IsVisible() const;

protected:
    virtual void onSpawn() override;

private:
    float   mWidth;
    float   mHeight;

    Mode    mMode;

    pt::math::float4    mColorBase;
    pt::math::float4    mColorFrame;
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
};


}

