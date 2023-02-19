#include "engine/test/ActorTester/ColorRectComponent.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;
using namespace pt::math;

std::vector<pt::math::float3> ColorRectComponent::CRCColors = std::vector<pt::math::float3>();
bool ColorRectComponent::Locked = false;
int32_t ColorRectComponent::LastID = -1;


ColorRectComponent::
ColorRectComponent(const std::string &name):
    SDLBillboardComponent( name )
{
    init();
}


ColorRectComponent::
ColorRectComponent( const ColorRectComponent& other ):
    SDLBillboardComponent( other )
{
    init();
}


ColorRectComponent::
ColorRectComponent( ColorRectComponent&& source ):
    SDLBillboardComponent( source )
{
    init();
}


ColorRectComponent::
~ColorRectComponent()
{}


ColorRectComponent &ColorRectComponent::
operator=( const ColorRectComponent& other )
{}


ColorRectComponent &ColorRectComponent::
operator=( ColorRectComponent&& source )
{}


bool ColorRectComponent::
operator==( const ColorRectComponent& other ) const
{
    assert(false); //TODO: implement
    return false;
}


void ColorRectComponent::
Tick( float t, float dt )
{
    //if our ID in invalid, we don't change colors
    if( mID < 0 ){
        return;
    }

    //if a second has not passed since last update
    if( t < mLastUpdateT + 1.0f ){
        return;
    }

    mLastUpdateT = t;

    if( Locked ){
        return;
    }


    //if the LastID is default (we are the first to write)
    if(LastID < 0){
        LastID = mID;
        SetNextColor();
    }else{
        //if the order is sound
        if( LastID < mID){
            LastID = mID;
            SetNextColor();
        }else{
            //either we were the last to modify it
            //  or the order is no longer sound and someone dependant on us ticked earlier than us

            Locked = true;
            mColor = float3::red;
            this->SetBaseColor(mColor);
        }
    }
}


void ColorRectComponent::
Draw(float t, float dt)
{
    SDLBillboardComponent::Draw(t, dt );

}

void ColorRectComponent::
SetNextColor()
{
    size_t colorcount = CRCColors.size();
    mIdxColor = (++mIdxColor) % colorcount;
    mColor = CRCColors[mIdxColor];
    this->SetBaseColor(mColor);
}

float3 ColorRectComponent::
GetColor() const
{
    return CRCColors[mIdxColor];
}

void ColorRectComponent::
SetID(int32_t id)
{
    mID = id;
    mIdxColor = mID % CRCColors.size();
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
OnSpawned()
{
    SDLBillboardComponent::OnSpawned();

}


void ColorRectComponent::
OnDespawned()
{
    SDLBillboardComponent::OnDespawned();

}


bool ColorRectComponent::
OnCreateContext()
{
    bool success = SDLBillboardComponent::OnCreateContext();
    assert( success );
    if( !success ){
        //TODO: log error
        return false;
    }
    //-----
    //...

    //-----
    return true;
}


bool ColorRectComponent::
OnDestroyContext()
{
    //-----
    //...

    //-----
    bool success = SDLBillboardComponent::OnDestroyContext();
    assert( success );
    if( !success ){
        //TODO: log error
        return false;
    }
    return true;
}


void ColorRectComponent::
init()
{
    if(0 == CRCColors.size()){
        CRCColors.reserve(8);
        CRCColors.push_back(float3::aquamarine);
        CRCColors.push_back(float3::yellow);
        CRCColors.push_back(float3::green);
        //must not be red!
    }
}
