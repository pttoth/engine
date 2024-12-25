#include "engine/component/LightConeComponent.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"

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
        mUniColor       = vec4( mColor, 1.0f );
        mUniIntensity   = mIntensity;
        mUniAngle       = math::DegToRad( mAngle );
        mUniRadius      = mRadius;
        mUniEnabled     = mEnabled;

        auto dc = Services::GetRenderer();
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
OnRender_GL3_3( float t, float dt )
{}


bool LightConeComponent::
OnCreateRenderContext_GL3_3()
{
    auto dc = Services::GetRenderer();

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
    mUniColor     = shp->GetUniform<math::vec4>( lightObjectName + ".mColor" );
    mUniIntensity = shp->GetUniform<float>(      lightObjectName + ".mIntensity" );
    mUniAngle     = shp->GetUniform<float>(      lightObjectName + ".mAngle" );
    mUniRadius    = shp->GetUniform<float>(      lightObjectName + ".mRadius" );
    mUniEnabled   = shp->GetUniform<int>(        lightObjectName + ".mEnabled" );

    return true;
}


bool LightConeComponent::
OnDestroyRenderContext_GL3_3()
{
    auto dc = Services::GetRenderer();
    dc->ReleaseLightSlot( mLightSlot );

    return true;
}


void LightConeComponent::
OnTick( float t, float dt )
{
    LoadParametersToCurrentShader();
}

