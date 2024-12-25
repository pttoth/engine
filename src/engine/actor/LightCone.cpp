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


LightCone::
~LightCone()
{}


void LightCone::
UpdateLightParametersInCurrentShader()
{
    PT_WARN_DEPRECATED_FUNCTION
    auto lambda = [this]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->LoadParametersToCurrentShader();
    };
    this->PostMessage( lambda );
}


void LightCone::
Enable( bool val )
{
    auto lambda = [this, val]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->EnableLight( val );
    };
    this->PostMessage( lambda );
}


void LightCone::
SetAngle( float angle )
{
    auto lambda = [this, angle]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->SetAngle( angle );
    };
    this->PostMessage( lambda );

}


void LightCone::
SetColor( math::vec3 color )
{
    auto lambda = [this, color]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->SetColor( color );
    };
    this->PostMessage( lambda );
}


void LightCone::
SetIntensity( float intensity )
{
    auto lambda = [this, intensity]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->SetIntensity( intensity );
    };
    this->PostMessage( lambda );
}


void LightCone::
SetRadius( float radius )
{
    auto lambda = [this, radius]() -> void
    {
        pt::MutexLockGuard lock( mMutActorData );
        mLightCone->SetRadius( radius );
    };
    this->PostMessage( lambda );
}


void LightCone::
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


math::vec3 LightCone::
GetColor() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightCone->GetColor();
}


float LightCone::
GetIntensity() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightCone->GetIntensity();
}


float LightCone::
GetRadius() const
{
    pt::MutexLockGuard lock( mMutActorData );
    return mLightCone->GetRadius();
}


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
    pt::MutexLockGuard lock( mMutActorData );
    mLightCone->CreateRenderContext_GL3_3();
    mMesh->CreateRenderContext_GL3_3();
    mAxis->CreateRenderContext_GL3_3();

    return true;
}


void LightCone::
OnDestroyRenderContext()
{}
