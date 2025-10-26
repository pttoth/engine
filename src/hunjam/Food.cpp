#include "hunjam/Food.h"

#include <cstdlib>

using namespace hunjam;
using namespace engine;
using namespace math;


void Food::
Launch()
{
    static int i = 0;
    if( 0 == i ){
        std::srand(static_cast<unsigned>( std::time(nullptr) ));
        ++i;
    }

    mIsFlying           = true;
    size_t random       = std::rand() % mLaunchers.size();

    SetPosition( mLaunchers[random].mPos );
    math::vec3 forward  = ( mLaunchers[random].mTransform * vec4( vec3::xUnit, 1 ) ).XYZ();
    mInertia            = forward * mThrowForce;
}


Food::
Food( const std::string& name ):
    Actor( name )
{
    float scale = 100;

    mLastTriggerTime = 0;

    //mLaunchers.push_back( Launcher( vec3(1, 1, 1), FRotator( 45, 45, 0 ) ) );
    mLaunchers.push_back( Launcher( vec3(-100, -100, 50), FRotator( -65, 0, 0 ) ) );

    mAxis = NewPtr<AxisDisplayComponent>( "mAxis" );
    this->AddComponent_NoLock( mAxis );
    mAxis->SetParent( this->GetRootComponent_NoLock().get() );
    mAxis->SetScale( scale );


    mMesh = NewPtr<MeshComponent>( "mMesh" );
    this->AddComponent_NoLock( mMesh );
    mMesh->SetMesh( "hunjam/karo" );
    mMesh->SetParent( this->GetRootComponent_NoLock().get() );

    mMesh->SetScale( scale );

    mMeshCollider = NewPtr<MeshComponent>( "mMeshCollider" );
    mMeshCollider->SetMesh( "hunjam/karo_hb" );
    mMeshCollider->SetParent( mMesh.get() );

}



Food::
~Food()
{}


void Food::
OnTick( float t, float dt )
{
    if( mHasLanded ){
        if( mLandTime < 1 /* seconds */ ){
            mHasLanded      = false;
        }

    }else if( mIsFlying ){
        auto pos = GetPosition();
        if( pos.z < 0 ){
            mIsFlying   = false;
            mHasLanded  = true;
            mLandTime   = 0; //now;
            mInertia    = math::vec3::zero;
        }else{
            vec3 velocity = mInertia * dt;
            SetPosition( pos + velocity );
            mInertia += mGravity * dt;
            // rotate mesh mid-air
            //GetRootComponent_NoLock()->LookAt( pos + mInertia );
        }

    }else{  //  is ready to be shot
        using namespace std::chrono;

        if ( 5 < t - mLastTriggerTime ) {
            mLastTriggerTime = t;
            Launch();
        }
    }
}


void Food::
OnSpawned()
{}


void Food::
OnDespawned()
{}


bool Food::
OnCreateRenderContext()
{
    return true;
}


void Food::
OnDestroyRenderContext()
{}
