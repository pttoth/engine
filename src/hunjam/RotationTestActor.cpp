#include "test/opengl_test/RotationTestActor.h"

using namespace engine;
using namespace math;

RotationTestActor::
RotationTestActor( const std::string& name ):
    Actor( name )
{
    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( GetRootComponent_NoLock().get() );
    mAxis->SetScale( vec3::one * 100 );

    mMesh = NewPtr<MeshComponent>( "mMesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetPosition( vec3( 0, 0, 0 ) );
    mMesh->SetParent( GetRootComponent_NoLock().get() );
    mMesh->SetScale( vec3::one * 10.0f );
    mMesh->SetMesh( "model/doom3/models/md5/monsters/cacodemon/cacodemon" );

}


RotationTestActor::
~RotationTestActor()
{}


void RotationTestActor::
OnTick( float t, float dt )
{}


void RotationTestActor::
OnSpawned()
{}


void RotationTestActor::
OnDespawned()
{}


bool RotationTestActor::
OnCreateRenderContext()
{
    return true;
}


void RotationTestActor::
OnDestroyRenderContext()
{}
