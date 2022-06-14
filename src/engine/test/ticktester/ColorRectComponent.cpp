#include "engine/test/ticktester/ColorRectComponent.h"

#include <iostream>
#include <assert.h>

using namespace pt::math;


ColorRectComponent::
ColorRectComponent(const std::string& name, int32_t id):
    SDLBillboardComponent(name),
    mID(id)
{
    if(0 == CRCColors.size()){
        CRCColors.reserve(8);
        CRCColors.push_back(float3::aquamarine);
        CRCColors.push_back(float3::yellow);
        CRCColors.push_back(float3::green);
        //must not be red!

        ResetLastID();
        Locked = false;
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
    //if our ID in invalid, we don't change colors
    if( mID < 0 ){
        return;
    }

    //if a second has not passed since last update
    if( t < mLastUpdateT + 1000.0f ){
        return;
    }

    if( Locked ){
        return;
    }


    //if the LastID is default (we are the first to write)
    if(LastID < 0){
        LastID = mID;
        SetNextColor();
    }else{
        //if the order is sound
        if( mID < LastID){
            LastID = mID;
            SetNextColor();
        }else{
            //either we were the last to modify it
            //  or the order is no longer sound and someone dependant on us ticked earlier than us

            Locked = true;
            mColor = float3::red;
        }
    }
}


void ColorRectComponent::
OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void ColorRectComponent::
OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid)
{}


void ColorRectComponent::
OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid)
{}


void ColorRectComponent::
OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction)
{}


void ColorRectComponent::
OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


void ColorRectComponent::
OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat)
{}


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
SetID(int32_t id)
{
    mID = id;
}


int32_t ColorRectComponent::
GetID() const
{
    return mID;
}

void ColorRectComponent::
ResetLastID()
{
    LastID = -1;
}


void ColorRectComponent::
onSpawn()
{}
