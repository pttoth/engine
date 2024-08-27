#include "engine/component/LightConeComponent.h"

#include "engine/Services.h"
#include "engine/service/DrawingControl.h"

using namespace engine;


LightConeComponent::
LightConeComponent( const std::string& name ):
    LightComponentBase( name )
{}


LightConeComponent::
~LightConeComponent()
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
        mUniEnabled     = mEnabled;

        auto dc = Services::GetDrawingControl();
        auto shp = dc->GetCurrentShaderProgram();
        shp->SetUniform( mUniTransform );
        shp->SetUniform( mUniColor );
        shp->SetUniform( mUniIntensity );
        shp->SetUniform( mUniAngle );
        shp->SetUniform( mUniRadius);
        shp->SetUniform( mUniEnabled );

        //mParamsDirty = false; //@TODO: re-enable later | shader switching invalidates logic!
    }
}


void LightConeComponent::
EnableLight( bool val )
{
    mEnabled = val;
    mParamsDirty = true;
}


bool LightConeComponent::
IsLightEnabled() const
{
    return mEnabled;
}


void LightConeComponent::
SetColor( math::vec3 color )
{
    mColor = color;
    mParamsDirty = true;
}


void LightConeComponent::
SetIntensity( float intensity )
{
    mIntensity = intensity;
    mParamsDirty = true;
}


void LightConeComponent::
SetRadius( float radius )
{
    mRadius = radius;
    mParamsDirty = true;
}


void LightConeComponent::
SetAngle( float angle )
{
    mAngle = angle;
    mParamsDirty = true;
}


math::vec3 LightConeComponent::
GetColor() const
{
    return mColor;
}


float LightConeComponent::
GetIntensity() const
{
    return mIntensity;
}


float LightConeComponent::
GetRadius() const
{
    return mRadius;
}


float LightConeComponent::
GetAngle() const
{
    return mAngle;
}


void LightConeComponent::
OnDraw( float t, float dt )
{}


bool LightConeComponent::
OnCreateContext()
{
    auto dc = Services::GetDrawingControl();

    if( mLightSlot < 0 ){
        mLightSlot = dc->GetLightSlot();
        PT_LOG_DEBUG( "Cone Light slot '" << mLightSlot << "' allocated to '" << this->GetName() << "'" );
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
    mUniColor     = shp->GetUniform<math::vec3>( lightObjectName + ".mColor" );
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
    LoadParametersToCurrentShader();
}

