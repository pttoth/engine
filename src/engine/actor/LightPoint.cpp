#include "engine/actor/LightPoint.h"

using namespace engine;

LightPoint::
LightPoint( const std::string& name ):
    Actor( name )
{
    this->GetRootComponent_NoLock()->SetPosition( 0, 0, 0 );

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( GetRootComponent_NoLock().get() );
    mAxis->SetScale( 100 );

    mLightPointComponent = NewPtr<LightPointComponent>( "mLightPointComponent" );
    this->AddComponent_NoLock( mLightPointComponent );
    mLightPointComponent->SetParent( GetRootComponent_NoLock().get() );

    mMesh = NewPtr<MeshComponent>( "mMesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetMesh( "dev_camera" );
    mMesh->SetScale( 20 );
    mMesh->SetParent( GetRootComponent_NoLock().get() );
}


LightPoint::
~LightPoint()
{}


void LightPoint::
UpdateLightParametersInCurrentShader()
{
    PT_WARN_DEPRECATED_FUNCTION
    auto lambda = [this]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightPointComponent->LoadParametersToCurrentShader();
    };
    this->PostMessage( lambda );
}


void LightPoint::
Enable( bool val )
{
    auto lambda = [this, val]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightPointComponent->EnableLight( val );
    };
    this->PostMessage( lambda );
}


void LightPoint::
SetColor( math::vec3 color )
{
    auto lambda = [this, color]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightPointComponent->SetColor( color );
    };
    this->PostMessage( lambda );
}


void LightPoint::
SetIntensity( float intensity )
{
    auto lambda = [this, intensity]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightPointComponent->SetIntensity( intensity );
    };
    this->PostMessage( lambda );
}


void LightPoint::
SetRadius( float radius )
{
    auto lambda = [this, radius]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightPointComponent->SetRadius( radius );
    };
    this->PostMessage( lambda );
}


void LightPoint::
ShowMesh( bool val )
{
    auto lambda = [this, val]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mShowMesh = val;
        if( mShowMesh && (nullptr != mMesh) ){
            mMesh->EnableDraw( mShowMesh );
        }
    };
    this->PostMessage( lambda );
}


math::vec3 LightPoint::
GetColor() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightPointComponent->GetColor();
}


float LightPoint::
GetIntensity() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightPointComponent->GetIntensity();
}


float LightPoint::
GetRadius() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightPointComponent->GetRadius();
}


void LightPoint::
OnTick( float t, float dt )
{}


void LightPoint::
OnSpawned()
{}


void LightPoint::
OnDespawned()
{}


bool LightPoint::
OnCreateRenderContext()
{
    pt::MutexLockGuard lock( mMutActorData );
    mLightPointComponent->CreateContext();
    mMesh->CreateContext();
    mAxis->CreateContext();

    return true;
}


void LightPoint::
OnDestroyRenderContext()
{}
