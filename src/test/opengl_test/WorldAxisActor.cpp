#include "test/opengl_test/WorldAxisActor.h"

//using namespace ClassNameSpace;

using namespace engine;
using namespace math;

WorldAxisActor::WorldAxisActor( const std::string& name ):
    Actor( name )
{
    mAxisComponent = NewPtr<engine::AxisDisplayComponent>( "mAxisComponent" );

    this->AddDrawableComponent_NoLock( mAxisComponent );

    mAxisComponent->SetParent( GetRootComponent_NoLock().get() );
    //mAxisComponent->SetScale( vec3::one * 100000.0f );

}


WorldAxisActor::
~WorldAxisActor()
{}


void WorldAxisActor::OnTick(float t, float dt)
{

}

void WorldAxisActor::OnSpawned()
{
}

void WorldAxisActor::OnDespawned()
{

}

bool WorldAxisActor::OnCreateRenderContext()
{
    mAxisComponent->CreateContext();

    return true;
}

void WorldAxisActor::OnDestroyRenderContext()
{

}
