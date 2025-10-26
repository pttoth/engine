#include "hunjam/StakeActor.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"

using namespace hunjam;
using namespace engine;


void StakeActor::
Shoot()
{

}


void StakeActor::
Return()
{

}


StakeActor::
StakeActor( const std::string& name ):
    Actor( name )
{
    float scale = 100;

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( this->GetRootComponent_NoLock().get() );
    mAxis->SetScale( scale );

    mMesh = NewPtr<MeshComponent>( "ViewModel_Mesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetMesh( "model/doom3/models/md5/weapons/plasmagun_view/viewplasmagun" );
    mMesh->SetPreferredDrawStage( gl::RenderStage::WPN_VIEWMODEL );
    mMesh->SetParent( this->GetRootComponent_NoLock().get() );

    mMesh->SetScale( scale );
    mMesh->SetPosition( 100, 0, -25 );
    mMesh->SetRotation( math::FRotator( 90, 0, 0 ) );
}


StakeActor::
~StakeActor()
{}


void StakeActor::
OnTick( float t, float dt )
{
    auto camera = engine::Services::GetRenderer()->GetMainCamera();
    //mMesh->SetWorldTransform( mMesh->GetWorldTransform() * mOrientOverride );
    this->SetWorldTransform( camera->GetWorldTransform() );
    //this->SetWorldTransform( this->GetWorldTransform() * mOrientOverride );

    if( !mIsFlying ){

    }


}


void StakeActor::
OnSpawned()
{}


void StakeActor::
OnDespawned()
{}


bool StakeActor::
OnCreateRenderContext()
{
    return true;
}


void StakeActor::
OnDestroyRenderContext()
{}
