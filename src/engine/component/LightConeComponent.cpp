#include "engine/component/LightConeComponent.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/DrawingControl.h"

using namespace engine;


LightConeComponent::
LightConeComponent( const std::string& name ):
    RealComponent( name )
{}


void LightConeComponent::
LoadParametersToCurrentShader()
{
    if( mParamsDirty ){
        mUniTransform   = this->GetWorldTransform();
        mUniColor       = mColor;
        mUniIntensity   = mIntensity;
        mUniAngle       = math::DegToRad( mAngle );
        mUniRadius      = mRadius;
        mUniEnabled     = 1;

        auto dc = Services::GetDrawingControl();
        auto shp = dc->GetCurrentShaderProgram();
        shp->SetUniform( mUniTransform );
        shp->SetUniform( mUniColor );
        shp->SetUniform( mUniIntensity );
        shp->SetUniform( mUniAngle );
        shp->SetUniform( mUniRadius);
        shp->SetUniform( mUniEnabled );

        mParamsDirty = false;
    }
}


void LightConeComponent::
SetAngle( float val )
{
    mAngle = val;
    mParamsDirty = true;
}


void LightConeComponent::
OnDraw( float t, float dt )
{
    // @TODO: delete this later (after Actor calls it)
    LoadParametersToCurrentShader();
}


bool LightConeComponent::
OnCreateContext()
{
    auto dc = Services::GetDrawingControl();

    if( mLightSlot < 0 ){
        mLightSlot = dc->GetLightSlot();
    }

    std::string lightObjectName;
    {
        std::stringstream ss;
        //ss << "LightingInfo.coneLights[" << mLightSlot << "]";
        ss << "coneLights[" << mLightSlot << "]";
        lightObjectName = ss.str();
    }

    auto shp = dc->GetCurrentShaderProgram();

    mUniTransform = shp->GetUniform<math::mat4>( lightObjectName + ".mTransform" );
    mUniColor     = shp->GetUniform<math::vec4>( lightObjectName + ".mColor" );
    mUniIntensity = shp->GetUniform<float>(      lightObjectName + ".mIntensity" );
    mUniAngle     = shp->GetUniform<float>(      lightObjectName + ".mAngle" );
    mUniRadius    = shp->GetUniform<float>(      lightObjectName + ".mRadius" );
    mUniEnabled   = shp->GetUniform<int>(        lightObjectName + ".mEnabled" );

    return true;
}


bool LightConeComponent::
OnDestroyContext()
{
    auto dc = Services::GetDrawingControl();
    dc->ReleaseLightSlot( mLightSlot );

    return true;
}


void LightConeComponent::
OnTick( float t, float dt )
{

}


LightConeComponent::
~LightConeComponent()
{}

