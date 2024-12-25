#include "test/opengl_test/WorldGeometry.h"

using namespace math;
using namespace engine;

WorldGeometry::
WorldGeometry( const std::string& name ):
    Actor( name )
{
    std::string meshNamePillar      = "model/dev/testmap1/pillar1";
    std::string meshNamePavement    = "model/dev/testmap1/pavement1";
    std::string meshNameWall        = "model/dev/testmap1/wall1";

    std::string rootMeshName        = "RootPavement";

    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( this->GetRootComponent_NoLock()->GetName(), rootMeshName ) );
        mc->SetMesh( meshNamePavement );
        //mc->SetScale( 100.0f );
        mMeshes.push_back( mc );
    }

    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( rootMeshName, "Wall_North" ) );
        mc->SetPosition( vec3(40.0f, 0.0f, 9.0f) );
        mc->SetRotation( FRotator( 0, 90, 0 ) );
        mc->SetMesh( meshNameWall );
        mMeshes.push_back( mc );
    }

    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( rootMeshName, "Wall_South" ) );
        mc->SetPosition( vec3(-40.0f, 0.0f, 9.0f) );
        mc->SetRotation( FRotator( 0, 90, 0 ) );
        mc->SetMesh( meshNameWall );
        mMeshes.push_back( mc );
    }

    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( rootMeshName, "Wall_West" ) );
        mc->SetPosition( vec3(0.0f, 40.0f, 9.0f) );
        mc->SetMesh( meshNameWall );
        mMeshes.push_back( mc );
    }

    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( rootMeshName, "Wall_East" ) );
        mc->SetPosition( vec3(0.0f, -40.0f, 9.0f) );
        mc->SetMesh( meshNameWall );
        mMeshes.push_back( mc );
    }



    {
        MeshComponentPtr mc = NewPtr<MeshComponent>( Component::GenerateComponentName( rootMeshName, "Column.001" ) );
        mc->SetMesh( meshNamePavement );
        mMeshes.push_back( mc );
    }


    for( auto mc : mMeshes ){
        float scale = 50.0f;
        mc->SetScale( scale );
        mc->SetPosition( mc->GetPosition() * scale );
        this->AddComponent_NoLock( mc );
    }

}


WorldGeometry::~WorldGeometry()
{}


void WorldGeometry::
TickComponents_NoDelay(float t, float dt)
{
    Actor::TickComponents_NoDelay( t,dt );
}


void WorldGeometry::
OnPreTickComponents(float t, float dt)
{
    Actor::OnPreTickComponents( t,dt );
}


void WorldGeometry::
OnPostTickComponents(float t, float dt)
{
    Actor::OnPostTickComponents( t,dt );
}


void WorldGeometry::
OnTick(float t, float dt)
{}


void WorldGeometry::
OnSpawned()
{}


void WorldGeometry::
OnDespawned()
{}


bool WorldGeometry::
OnCreateRenderContext()
{
    for( auto mc : mMeshes ){
        mc->CreateRenderContext_GL3_3();
    }
    return true;
}


void WorldGeometry::
OnDestroyRenderContext()
{}
