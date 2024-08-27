#include "engine/component/LightPointComponent.h"

#include "engine/Services.h"
#include "engine/service/DrawingControl.h"

using namespace engine;

LightPointComponent::
LightPointComponent( const std::string& name ):
    LightComponentBase( name )
{}


LightPointComponent::
~LightPointComponent()
{}


void LightPointComponent::
LoadParametersToCurrentShader()
{
    if( mParamsDirty ){
        mUniPos         = this->GetWorldPosition();
        mUniColor       = mColor;
        mUniIntensity   = mIntensity;
        mUniRadius      = mRadius;
        mUniEnabled     = mEnabled;

        auto dc = Services::GetDrawingControl();
        auto shp = dc->GetCurrentShaderProgram();
        shp->SetUniform( mUniPos );
        shp->SetUniform( mUniColor );
        shp->SetUniform( mUniIntensity );
        shp->SetUniform( mUniRadius);
        shp->SetUniform( mUniEnabled );

        //mParamsDirty = false; //@TODO: re-enable later | shader switching invalidates logic!
    }
}


void LightPointComponent::
EnableLight( bool val )
{
    mEnabled = val;
    mParamsDirty = true;
}


bool LightPointComponent::
IsLightEnabled() const
{
    return mEnabled;
}


void LightPointComponent::
SetColor( math::vec3 color )
{
    mColor = color;
    mParamsDirty = true;
}


void LightPointComponent::
SetIntensity( float intensity )
{
    mIntensity = intensity;
    mParamsDirty = true;
}


void LightPointComponent::
SetRadius( float radius )
{
    mRadius = radius;
    mParamsDirty = true;
}


math::vec3 LightPointComponent::
GetColor() const
{
    return mColor;
}


float LightPointComponent::
GetIntensity() const
{
    return mIntensity;
}


float LightPointComponent::
GetRadius() const
{
    return mRadius;
}


void LightPointComponent::
OnDraw( float t, float dt )
{}


bool LightPointComponent::
OnCreateContext()
{
    auto dc = Services::GetDrawingControl();

    if( mLightSlot < 0 ){
        mLightSlot = dc->GetLightSlot();
        PT_LOG_DEBUG( "Point Light slot '" << mLightSlot << "' allocated to '" << this->GetName() << "'" );
    }

    std::string lightObjectName;
    {
        std::stringstream ss;
        //ss << "LightingInfo.pointLights[" << mLightSlot << "]";
        ss << "pointLights[" << mLightSlot << "]";
        lightObjectName = ss.str();
    }

    auto shp = dc->GetCurrentShaderProgram();

    mUniPos       = shp->GetUniform<math::vec3>( lightObjectName + ".mPos" );
    mUniColor     = shp->GetUniform<math::vec3>( lightObjectName + ".mColor" );
    mUniIntensity = shp->GetUniform<float>(      lightObjectName + ".mIntensity" );
    mUniRadius    = shp->GetUniform<float>(      lightObjectName + ".mRadius" );
    mUniEnabled   = shp->GetUniform<int>(        lightObjectName + ".mEnabled" );

    return true;
}


bool LightPointComponent::
OnDestroyContext()
{
    return true;
}


void LightPointComponent::
OnTick( float t, float dt )
{
    // @TODO: delete this later (after Actor calls it)
    LoadParametersToCurrentShader();
}
