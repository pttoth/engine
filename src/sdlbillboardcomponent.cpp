#include "sdlbillboardcomponent.h"

using namespace engine;


SDLBillboardComponent::
SDLBillboardComponent():
    mWidth( 0.0f ), mHeight( 0.0f ),
    mMode( Mode::FilledRGBA ),
    mColorBase( pt::math::float3::white ), mColorBaseAlpha( 1.0f ),
    mColorFrame( pt::math::float3::white ), mColorFrameAlpha( 1.0f )
    //type...           mProcedural;      //TODO
    //Texture           mTexture;         //TODO
{}


SDLBillboardComponent::
~SDLBillboardComponent()
{}


bool SDLBillboardComponent::
operator==( const SDLBillboardComponent &other )
{}


void SDLBillboardComponent::
tick( float t, float dt )
{}


void SDLBillboardComponent::
OnRegistered()
{}


void SDLBillboardComponent::
OnUnregistered()
{}


void SDLBillboardComponent::
OnCreateContext()
{}


void SDLBillboardComponent::
OnDestroyContext()
{}


void SDLBillboardComponent::
Draw()
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


void SDLBillboardComponent::
SetBaseColor(pt::math::float3 color)
{
    mColorBase = color;
}


void SDLBillboardComponent::
SetBaseColorAlpha(float alpha)
{
    mColorBaseAlpha = alpha;
}


void SDLBillboardComponent::
SetFrameColor(pt::math::float3 color)
{
    mColorFrame = color;
}


void SDLBillboardComponent::
SetFrameColorAlpha(float alpha)
{
    mColorFrameAlpha = alpha;
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


pt::math::float3 SDLBillboardComponent::
GetBaseColor() const
{
    mColorBase;
}


float SDLBillboardComponent::
GetBaseColorAlpha() const
{
    mColorBaseAlpha;
}


pt::math::float3 SDLBillboardComponent::
GetFrameColor() const
{
    mColorFrame;
}


float SDLBillboardComponent::
GetFrameColorAlpha() const
{
    return mColorFrameAlpha;
}


void SDLBillboardComponent::
OnSpawn()
{}
