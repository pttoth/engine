#include "test/opengl_test/PlasmaGun.h"

using namespace engine;

PlasmaGun::
PlasmaGun( const std::string& name ):
    Actor( name )
{
    float scale = 50;

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddDrawableComponent_NoLock( mAxis );
    mAxis->SetParent( this->GetRootComponent_NoLock().get() );
    mAxis->SetScale( scale );

    mMesh = NewPtr<MeshComponent>( "ViewModel_Mesh" );
    this->AddDrawableComponent_NoLock( mMesh );
    mMesh->SetParent( this->GetRootComponent_NoLock().get() );
    mMesh->SetMesh( "model/doom3/models/md5/weapons/plasmagun_view/viewplasmagun" );
    mMesh->SetScale( scale );
    mMesh->SetPosition( 0,0, 75 );
}


PlasmaGun::
~PlasmaGun()
{}


void PlasmaGun::
OnTick( float t, float dt )
{}


void PlasmaGun::
OnSpawned()
{}


void PlasmaGun::
OnDespawned()
{}


bool PlasmaGun::
OnCreateRenderContext()
{
    mAxis->CreateContext();
    mMesh->CreateContext();
    return true;
}


void PlasmaGun::
OnDestroyRenderContext()
{}
