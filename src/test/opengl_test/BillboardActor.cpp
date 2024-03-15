#include "test/opengl_test/BillboardActor.h"

#include "engine/gl/Buffer.hpp"

using namespace engine;

BillboardActor::
BillboardActor( const std::string& name ):
    engine::Actor( name )
{
    mBillboardComponent = NewPtr<BillboardComponent>( "BillboardComponent" );
    Actor::AddDrawableComponent_NoLock( mBillboardComponent );
    mBillboardComponent->SetParent( this->GetRootComponent_NoLock().get() );
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
