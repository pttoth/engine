#include "test/opengl_test/BillboardActor.h"

#include "engine/gl/Buffer.hpp"

using namespace engine;
using namespace math;

BillboardActor::
BillboardActor( const std::string& name ):
    engine::Actor( name )
{
    mBillboardComponent = NewPtr<BillboardComponent>( "BillboardComponent" );

    this->AddDrawableComponent_NoLock( mBillboardComponent );
    mBillboardComponent->SetParent( GetRootComponent_NoLock().get() );
    mBillboardComponent->SetScale( vec3::one * 10.0f );
    mActorAxis = NewPtr<AxisDisplayComponent>( "BillboardActorAxisComponent" );

    this->AddDrawableComponent_NoLock( mActorAxis );
    mActorAxis->SetParent( GetRootComponent_NoLock().get() );
    mActorAxis->SetScale( vec3::one * 100 );

    mMeshComponent = NewPtr<MeshComponent>( "MeshComponent" );
    this->AddDrawableComponent_NoLock( mMeshComponent);
    mMeshComponent->SetPosition( vec3( 0, 0, 2.0f ) );
    //mMeshComponent->SetParent( mBillboardComponent.get() );
    mMeshComponent->SetParent( GetRootComponent_NoLock().get() );
    mMeshComponent->SetScale( vec3::one * 10.0f );
    mMeshComponent->SetMesh( "model/campbell/campbell" );
    //mMeshComponent->SetMesh( "model/doom3/models/md5/monsters/cacodemon/cacodemon" );
    //mMeshComponent->SetMeshPath( "model/wraith_silhouette" );

}


BillboardActor::
~BillboardActor()
{}


void BillboardActor::
SetTexture( gl::Texture2dPtr texture )
{
    mBillboardComponent->SetTexture( texture );
}


void BillboardActor::
SetMesh( std::string& name )
{
    mMeshComponent->SetMesh( name );
}


bool BillboardActor::
OnCreateRenderContext()
{
    mBillboardComponent->CreateContext();
    mActorAxis->CreateContext();

    mMeshComponent->CreateContext();
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
{
    PT_LOG_DEBUG( "BillboardActor::OnSpawned() called" );
}


void BillboardActor::
OnDespawned()
{}
