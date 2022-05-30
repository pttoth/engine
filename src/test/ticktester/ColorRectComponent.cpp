#include "test/ticktester/ColorRectComponent.h"

#include <iostream>
#include <assert.h>

std::vector<pt::math::float3> CRCColors = std::vector<pt::math::float3>();

using namespace pt::math;


ColorRectComponent::
ColorRectComponent(const std::string& name):
    SDLBillboardComponent(name)
{
    if(0 == CRCColors.size()){
        CRCColors.push_back(float3::aquamarine);
        CRCColors.push_back(float3::yellow);
        CRCColors.push_back(float3::green);
        //must not be red!
    }
}


ColorRectComponent::
ColorRectComponent(const ColorRectComponent& other):
    SDLBillboardComponent(other)
{}


ColorRectComponent::
~ColorRectComponent()
{}


bool ColorRectComponent::
operator==(const ColorRectComponent &other) const
{
    assert(false); //TODO: implement
    return false;
}


void ColorRectComponent::
tick(float t, float dt)
{
    /*
    if( mLocked ){
        return;
    }

    ColorRectComponent* parent = dynamic_cast<ColorRectComponent*>( this->GetParent() );
    //if: has no parent
    //  select a new color
    if( nullptr == parent ){
        SetNextColor();
        this->SetBaseColor(mColor);
        return;
    }

    //else
    //  (expects, that 'this' tick-depends on its parent)
    //  check, whether our color is the same as our parents'
    //      (meaning, we are ticking earlier, than them)
    //      if yes, lock to a red color
    float3 color = parent->GetColor();
    bool3 equality = ( color == this->GetColor() );

    bool colors_differ = false;
    for(int i = 0; i<3; ++i){
        if( !equality[i] ){
            colors_differ = true;
        }
    }

    if( !colors_differ ){
        mColor = float3::red;
        mLocked = true;
    }
    this->SetBaseColor(mColor);
    */
}


void ColorRectComponent::
OnRegistered()
{

}


void ColorRectComponent::
OnUnregistered()
{

}


void ColorRectComponent::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{

}


void ColorRectComponent::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{

}


void ColorRectComponent::
OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{

}


void ColorRectComponent::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{

}


void ColorRectComponent::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}


void ColorRectComponent::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{

}


void ColorRectComponent::
SetNextColor()
{
    size_t colorcount = CRCColors.size();
    mIdxColor = (++mIdxColor) % colorcount;
    mColor = CRCColors[mIdxColor];
}


pt::math::float3 ColorRectComponent::
GetColor() const
{
    return CRCColors[mIdxColor];
}


void ColorRectComponent::
onSpawn()
{}
