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
SetAngle(float val)
{
    mAngle = val;
}


void LightConeComponent::
OnDraw( float t, float dt )
{
    using mat4 = math::mat4;
    auto dc = Services::GetDrawingControl();
    auto shp = dc->GetCurrentShaderProgram();

    //@TODO: optimize

    mUniTransform   = this->GetWorldTransform();
    mUniColor       = mColor;
    mUniIntensity   = mIntensity;
    mUniAngle       = math::DegToRad( mAngle ); // @TODO: use degrees and convert on GPU?
    //mUniAngle       = mAngle; // @TODO: use degrees and convert on GPU?
    mUniRadius      = mRadius;
    mUniEnabled     = 1;

    static size_t ctr = 0;
    if( ctr %100 == 0 ){
        mat4 spotTf             = this->GetWorldTransform();
        vec4 spotPos = vec4( spotTf.m[0][3],
                             spotTf.m[1][3],
                             spotTf.m[2][3],
                             1 );
        vec4 spotDir            = (spotTf * vec4(1,0,0,1) - spotPos).normalize();
        vec4 spotDirLight       = (spotTf * vec4( vec3(1,1,1)*1000, 1 ) - spotPos).normalize();
        //spotDirLight            = vec4(spotDirLight.XYZ().normalize(), 1);
        float lightAngleSource  = acosf( spotDir.dot(spotDirLight) );
        float spotAngle         = mAngle;
/*
        PT_LOG_DEBUG( ToString( spotTf ) );
        PT_LOG_DEBUG( "spotDir:          " << ToString( spotDir ) );
        PT_LOG_DEBUG( "spotDirLight:     " << ToString( spotDirLight ) );
        PT_LOG_DEBUG( "lightAngleSource: " << math::RadToDeg( lightAngleSource ) );
        PT_LOG_DEBUG( "spotAngle:        " << spotAngle );
*/
    }

    shp->SetUniform( mUniTransform );
    shp->SetUniform( mUniColor );
    shp->SetUniform( mUniIntensity );
    shp->SetUniform( mUniAngle );
    shp->SetUniform( mUniRadius);
    shp->SetUniform( mUniEnabled );
}


bool LightConeComponent::
OnCreateContext()
{
    //auto ac = Services::GetAssetControl();
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

