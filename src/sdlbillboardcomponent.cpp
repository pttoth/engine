#include "sdlbillboardcomponent.h"

using namespace engine;


SDLBillboardComponent::
SDLBillboardComponent():
    mWidth(0.0f), mHeight(0.0f),
    mMode(Mode::FilledRGBA),
    mColorBase( pt::math::float4( pt::math::float3::white, 1.0f ) ),
    mColorFrame( pt::math::float4( pt::math::float3::white, 1.0f ) )
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
{}


SDLBillboardComponent::
~SDLBillboardComponent()
{}


bool SDLBillboardComponent::
operator==(const SDLBillboardComponent &other)
{}


void SDLBillboardComponent::
tick(float t, float dt)
{}


void SDLBillboardComponent::
OnRegistered()
{}


void SDLBillboardComponent::
OnUnregistered()
{}


void SDLBillboardComponent::
draw()
{}


bool SDLBillboardComponent::
IsVisible() const
{
    return true; //TODO: implement
}


void SDLBillboardComponent::
SetMode(SDLBillboardComponent::Mode mode)
{
    mMode = mode;
}


void SDLBillboardComponent::
SetHeight(float height)
{
    mHeight = height;
}


void SDLBillboardComponent::
SetWidth(float width)
{
    mWidth = width;
}


SDLBillboardComponent::Mode SDLBillboardComponent::
GetMode() const
{
    return mMode;
}


float SDLBillboardComponent::
GetHeight() const
{
    return mHeight;
}


float SDLBillboardComponent::
GetWidth() const
{
    return mWidth;
}


void SDLBillboardComponent::
onSpawn()
{}
