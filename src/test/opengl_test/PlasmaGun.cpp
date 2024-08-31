#include "test/opengl_test/PlasmaGun.h"

#include "engine/Services.h"
#include "engine/service/EngineControl.h"

using namespace engine;
using namespace math;

PlasmaGun::
PlasmaGun( const std::string& name ):
    Actor( name )
{
    float scale = 250;

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddDrawableComponent_NoLock( mAxis );
    mAxis->SetParent( this->GetRootComponent_NoLock().get() );
    mAxis->SetScale( scale );

    mMesh = NewPtr<MeshComponent>( "ViewModel_Mesh" );
    this->AddDrawableComponent_NoLock( mMesh );
    mMesh->SetMesh( "model/doom3/models/md5/weapons/plasmagun_view/viewplasmagun" );
    mMesh->SetPreferredDrawStage( gl::DrawStage::WPN_VIEWMODEL );
    mMesh->SetParent( this->GetRootComponent_NoLock().get() );

    mMesh->SetScale( scale );
    mMesh->SetPosition( 0, 0, 350 );

    for( auto& e : mProjectileSpawntime ){
        e = INT64_MAX;
    }
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

        BillboardComponentPtr& currentproj_billboard = mProjectileBillboards[idx];
        currentproj_billboard->EnableDraw( mEnableBillboards );
    }
}


void PlasmaGun::
KillOldestProjectile()
{
    int64_t oldest_spawntime = INT64_MAX;
    int oldest_idx = -1;
    for( int i=0; i<mMaxProjectileCount; ++i ){
        if( mProjectileLights[i]->IsLightEnabled() ){
            if( mProjectileSpawntime[i] < oldest_spawntime ){
                oldest_spawntime = mProjectileSpawntime[i];
                oldest_idx = i;
            }
        }
    }

    if( -1 < oldest_idx ){
        if( mProjectileLights[oldest_idx]->IsLightEnabled() ){
            KillProjectile( oldest_idx );
        }
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

        if( currentproj_light->IsLightEnabled() ){
            if( mMaxProjectileLifetime < t - mProjectileSpawntime[i] ){
                KillProjectile( i );
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

    // set up projectiles
    for( size_t i=0; i<mMaxProjectileCount; ++i ){
        std::stringstream ss;
        ss << "Projectile[" << i << "]";
        std::string prefix = ss.str();

        // set up projectile Lights
        ss.clear();
        ss << prefix << ".Light";
        mProjectileLights[i] = NewPtr<LightPointComponent>( ss.str() );
        LightPointComponentPtr& currentproj_light = mProjectileLights[i];

        this->AddComponent_NoLock( currentproj_light );
        //currentproj_light->SetColor( vec3::blue );
        currentproj_light->SetColor( vec3::teal );
        currentproj_light->SetIntensity( 20 );
        currentproj_light->SetRadius( 500 );
        currentproj_light->EnableLight( false );
        currentproj_light->CreateContext();

        // set up projectile Meshes
        ss.clear();
        ss << prefix << ".Mesh";
        mProjectileMeshes[i] = NewPtr<MeshComponent>( ss.str() );
        MeshComponentPtr& currentproj_mesh = mProjectileMeshes[i];

        this->AddComponent_NoLock( currentproj_mesh );
        currentproj_mesh->SetParent( currentproj_light.get() );
        currentproj_mesh->SetMesh( "model/dev/dev_plasmaprojectile" );
        currentproj_mesh->SetScale( 25 );
        currentproj_mesh->EnableDraw( false );
        currentproj_mesh->CreateContext();


        // set up projectile Billboards
        ss.clear();
        ss << prefix << ".Billboard";
        mProjectileBillboards[i] = NewPtr<BillboardComponent>( ss.str() );
        BillboardComponentPtr& currentproj_billboard = mProjectileBillboards[i];

        this->AddComponent_NoLock( currentproj_billboard );
        currentproj_billboard->SetParent( currentproj_light.get() );
        currentproj_billboard->SetScale( 25 );
        currentproj_billboard->SetTexture( "UnicolorTeal" );
        currentproj_billboard->AlwaysFaceCamera( true );
        currentproj_billboard->EnableDraw( mEnableBillboards );
        currentproj_billboard->CreateContext();
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


void PlasmaGun::
KillProjectile( size_t idx )
{
    if( mMaxProjectileCount < idx ){
        return;
    }

    LightPointComponentPtr& currentproj_light = mProjectileLights[idx];
    MeshComponentPtr& currentproj_mesh = mProjectileMeshes[idx];
    BillboardComponentPtr& currentproj_billboard = mProjectileBillboards[idx];
    PT_LOG_DEBUG( "projectile [" << idx << "] stopped" );
    currentproj_mesh->EnableDraw( false );
    currentproj_light->EnableLight( false );
    currentproj_billboard->EnableDraw( !mEnableBillboards );
    mProjectileSpawntime[idx] = 0;
}
