#include "test/opengl_test/BillboardActor.h"

#include "engine/gl/Buffer.hpp"

using namespace engine;
using namespace math;

BillboardActor::
BillboardActor( const std::string& name ):
    engine::Actor( name )
{
    mBillboardComponent = NewPtr<BillboardComponent>( "BillboardComponent" );
    Actor::AddDrawableComponent_NoLock( mBillboardComponent );
    mBillboardComponent->SetParent( GetRootComponent_NoLock().get() );

    mActorAxis = NewPtr<AxisDisplayComponent>( "ActorAxisComponent" );
    Actor::AddDrawableComponent_NoLock( mActorAxis );
    mActorAxis->SetParent( GetRootComponent_NoLock().get() );

    mAxis = NewPtr<AxisDisplayComponent>( "WorldAxisComponent" );
    Actor::AddDrawableComponent_NoLock( mAxis );
    mAxis->SetScale( vec3::one * 2 );
}


BillboardActor::
~BillboardActor()
{}


void BillboardActor::
SetTexture( gl::Texture2dPtr texture )
{
    mBillboardComponent->SetTexture( texture );
}


bool BillboardActor::
OnCreateRenderContext()
{

    return true;
}


void BillboardActor::
OnDestroyRenderContext()
{}


void BillboardActor::
OnTick( float t, float dt )
{}


void BillboardActor::
OnSpawned()
{}


void BillboardActor::
OnDespawned()
{}
