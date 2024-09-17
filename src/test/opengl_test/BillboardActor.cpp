#include "test/opengl_test/BillboardActor.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/gl/Buffer.hpp"

using namespace engine;
using namespace math;

BillboardActor::
BillboardActor( const std::string& name ):
    engine::Actor( name )
{
    mBillboardComponent = NewPtr<BillboardComponent>( "BillboardComponent" );

    this->AddComponent_NoLock( mBillboardComponent );
    mBillboardComponent->SetParent( GetRootComponent_NoLock().get() );
    mBillboardComponent->SetScale( vec3::one * 10.0f );
    //mBillboardComponent->SetScale( vec3::one * 100.0f );
    //mBillboardComponent->AlwaysFaceCamera( true );

    mActorAxis = NewPtr<AxisDisplayComponent>( "BillboardActorAxisComponent" );
    this->AddComponent_NoLock( mActorAxis );
    mActorAxis->SetParent( GetRootComponent_NoLock().get() );
    mActorAxis->SetScale( vec3::one * 100 );

    mMeshComponent = NewPtr<MeshComponent>( "MeshComponent" );
    this->AddComponent_NoLock( mMeshComponent);
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
    // @TODO: use message queue
    mBillboardComponent->SetTexture( texture );
}


void BillboardActor::
SetMesh( engine::gl::MeshPtr mesh )
{
    // @TODO: use message queue
    mMeshComponent->SetMesh( mesh );
}


void BillboardActor::
SetMesh( std::string& name )
{
    // @TODO: use message queue
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
