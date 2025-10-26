#include "hunjam/StaticActor.h"

using namespace engine;
using namespace hunjam;
using namespace math;

StaticActor::
StaticActor( const std::string& name ):
    Actor( name )
{
    float scale = 100;

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( this->GetRootComponent_NoLock().get() );
    mAxis->SetScale( scale );


    mMesh = NewPtr<MeshComponent>( "mMesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetParent( this->GetRootComponent_NoLock().get() );
    mMesh->SetScale( scale );

}


StaticActor::
~StaticActor()
{}


void StaticActor::
OnTick( float t, float dt )
{}


void StaticActor::
OnSpawned()
{}


void StaticActor::
OnDespawned()
{}


bool StaticActor::
OnCreateRenderContext()
{
    return true;
}


void StaticActor::
OnDestroyRenderContext()
{}
