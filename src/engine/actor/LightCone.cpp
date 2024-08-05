#include "engine/actor/LightCone.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/DrawingControl.h"

using namespace engine;
using namespace engine::gl;

LightCone::
LightCone( const std::string& name ):
    Actor( name )
{}


LightCone::
~LightCone()
{}


void LightCone::
OnTick( float t, float dt )
{

}


void LightCone::
OnSpawned()
{

}


void LightCone::
OnDespawned()
{

}


bool LightCone::
OnCreateRenderContext()
{
    auto ac = Services::GetAssetControl();
    auto dc = Services::GetDrawingControl();

    if( mLightSlot < 0 ){
        mLightSlot = dc->GetLightSlot();
    }




    return true;
}


void LightCone::
OnDestroyRenderContext()
{
    auto dc = Services::GetDrawingControl();
    dc->ReleaseLightSlot( mLightSlot );
}
