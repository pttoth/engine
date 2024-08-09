#include "engine/actor/LightCone.h"

using namespace engine;


LightCone::
LightCone( const std::string& name ):
    Actor( name )
{
    this->GetRootComponent_NoLock()->SetPosition( 0, 0, 0 );

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( GetRootComponent_NoLock().get() );
    mAxis->SetScale( 300 );

    mLightCone = NewPtr<LightConeComponent>( "mLightCone" );
    this->AddComponent_NoLock( mLightCone );
    mLightCone->SetParent( GetRootComponent_NoLock().get() );

    mMesh = NewPtr<MeshComponent>( "mMesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetMesh( "dev_camera" );
    mMesh->SetScale( 100 );
    mMesh->SetParent( GetRootComponent_NoLock().get() );
}


void LightCone::
ShowMesh( bool val )
{
    mShowMesh = val;
    if( mShowMesh && (nullptr != mMesh) ){
        mMesh->EnableDraw( mShowMesh );
    }
}


void LightCone::
SetAngle( float val )
{
    //@TODO: message queue
    mLightCone->SetAngle( val );
}


LightCone::
~LightCone()
{}


void LightCone::
OnTick( float t, float dt )
{}


void LightCone::
OnSpawned()
{}


void LightCone::
OnDespawned()
{}


bool LightCone::
OnCreateRenderContext()
{
    mLightCone->CreateContext();
    mMesh->CreateContext();

    return true;
}


void LightCone::
OnDestroyRenderContext()
{

}
