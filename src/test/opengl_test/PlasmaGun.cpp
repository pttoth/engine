#include "test/opengl_test/PlasmaGun.h"

#include "engine/Services.h"
#include "engine/service/EngineControl.h"

using namespace engine;
using namespace math;

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
    mMesh->SetPosition( 0, 0, 75 );


}


void PlasmaGun::
Shoot()
{
    assert( -1 < mSpawnedProjectiles );
    assert( mSpawnedProjectiles < mMaxProjectileCount );
    if( mSpawnedProjectiles < mMaxProjectileCount ){
        int32_t idx = FindFreeProjectileIndex();
        if( idx < 0 ){
            PT_LOG_DEBUG( "Firing failed, all projectile lights are in use." );
            return;
        }

        mProjectileSpawntime[idx] = Services::GetEngineControl()->GetCurrentTime() / 1000;

        LightPointComponentPtr& currentproj_light = mProjectileLights[idx];
        currentproj_light->SetPosition( GetProjectileSpawnLocation() );
        currentproj_light->SetRotation( GetRootComponent_NoLock()->GetRotationMtx() );
        currentproj_light->EnableLight( true );

        MeshComponentPtr& currentproj_mesh = mProjectileMeshes[idx];
        currentproj_mesh->EnableDraw( true );
    }
}


PlasmaGun::
~PlasmaGun()
{}


void PlasmaGun::
OnTick( float t, float dt )
{
    for( size_t i=0; i<mMaxProjectileCount; ++i ){
        LightPointComponentPtr& currentproj_light = mProjectileLights[i];
        MeshComponentPtr& currentproj_mesh = mProjectileMeshes[i];
        if( currentproj_light->IsLightEnabled() ){
            if( mMaxProjectileLifetime < t - mProjectileSpawntime[i] ){
                PT_LOG_DEBUG( "projectile [" << i << "] stopped" );
                currentproj_mesh->EnableDraw( false );
                currentproj_light->EnableLight( false );
            }else{
                mat4 xDeltaTf = mat4::identity;
                xDeltaTf.m[0][3] = mProjectileSpeed;
                mat4 currtf = currentproj_light->GetWorldTransform();
                currentproj_light->SetWorldTransform( currtf * xDeltaTf );
            }
        }
    }
}


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

    for( size_t i=0; i<mMaxProjectileCount; ++i ){
        std::stringstream ss;
        ss << "Projectile[" << i << "]";

        mProjectileLights[i] = NewPtr<LightPointComponent>( ss.str() );
        LightPointComponentPtr& currentproj_light = mProjectileLights[i];

        this->AddComponent_NoLock( currentproj_light );
        //currentproj_light->SetColor( vec3::blue );
        currentproj_light->SetColor( vec3::teal );
        currentproj_light->SetIntensity( 20 );
        currentproj_light->SetRadius( 500 );
        currentproj_light->EnableLight( false );
        currentproj_light->CreateContext();


        mProjectileMeshes[i] = NewPtr<MeshComponent>( ss.str() );
        MeshComponentPtr& currentproj_mesh = mProjectileMeshes[i];

        this->AddComponent_NoLock( currentproj_mesh );
        currentproj_mesh->SetParent( currentproj_light.get() );
        currentproj_mesh->SetMesh( "model/dev/dev_plasmaprojectile" );
        currentproj_mesh->SetScale( 25 );
        currentproj_mesh->EnableDraw( false );
        currentproj_mesh->CreateContext();

    }

    return true;
}


void PlasmaGun::
OnDestroyRenderContext()
{
    for( size_t i=0; i<mMaxProjectileCount; ++i ){
        mProjectileLights[i]->DestroyContext();
    }
    mMesh->DestroyContext();
    mAxis->DestroyContext();

}


vec3 PlasmaGun::
GetProjectileSpawnLocation() const
{
    mat4 xDeltaTf = mat4::identity;
    xDeltaTf.m[0][3] = 50;
    mat4 currtf = GetRootComponent_NoLock()->GetWorldTransform();
    mat4 retval = currtf * xDeltaTf;
    return vec3( retval.m[0][3],
                 retval.m[1][3],
                 retval.m[2][3] );
}


int32_t PlasmaGun::
FindFreeProjectileIndex() const
{
    for( size_t i=0; i<mMaxProjectileCount; ++i ){
        const LightPointComponentPtr& currentproj_light = mProjectileLights[i];

        if( not currentproj_light->IsLightEnabled() ){
            return i;
        }
    }
    return -1;
}
