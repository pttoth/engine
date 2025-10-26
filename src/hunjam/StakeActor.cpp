#include "hunjam/StakeActor.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"

using namespace hunjam;
using namespace engine;
using namespace math;

void StakeActor::
Shoot()
{
    mIsFlying       = true;
    auto forward    = GetRootComponent_NoLock()->GetForward();
    mInertia        = forward * mThrowForce;
    mMesh->SetPreferredDrawStage( gl::RenderStage::STANDARD );
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
    mMesh->SetPosition( mPositionOffset );
    mMesh->SetRotation( math::FRotator( 90, 0, 0 ) );

    mMeshAxis = NewPtr<AxisDisplayComponent>( "mMeshAxis" );
    this->AddComponent_NoLock( mMeshAxis );
    mMeshAxis->SetParent( mMesh.get() );
    mMeshAxis->SetScale( scale );

}


StakeActor::
~StakeActor()
{}


void StakeActor::
OnTick( float t, float dt )
{
    if( mHasLanded ){
        if( mLandTime < 1 /* seconds */ ){
            mHasLanded      = false;
            mIsReturning    = true;
        }

    }else if( mIsReturning ){
        auto    posCamera   = engine::Services::GetRenderer()->GetMainCamera()->GetPosition();
        auto    posStake    = this->GetPosition();
        auto    diff        = (posCamera - posStake);
        float   length      = diff.length();
        if( length < 1 /* seconds */ ){
            mIsReturning    = false;
            mMesh->SetPreferredDrawStage( gl::RenderStage::WPN_VIEWMODEL );
        }else{
            this->SetPosition( posStake + diff/4 );
        }

    }else if( mIsFlying ){
        auto pos = GetPosition();
        if( pos.z < 0 ){
            mIsFlying   = false;
            mHasLanded  = true;
            mLandTime   = 0; //now;
            mInertia    = math::vec3::zero;
        }else{
            SetPosition( pos + mInertia );
            mInertia += mGravity;
            // rotate mesh mid-air
            //GetRootComponent_NoLock()->LookAt( pos + mInertia );

            // TODO: hit detection
            // ...
        }

    }else{  //  is in hand
        auto camera = engine::Services::GetRenderer()->GetMainCamera();
        vec4 posOffsetToCamera = vec4(vec3(100, 0, -25), 1);
        mat4 mtranslation = mat4::identity;
        mtranslation.m[0][3] = posOffsetToCamera[0];
        mtranslation.m[1][3] = posOffsetToCamera[1];
        mtranslation.m[2][3] = posOffsetToCamera[2];
        mtranslation.m[3][3] = posOffsetToCamera[3];

        mat4 mtx = mtranslation * math::FRotator( 0, 0, 0 ).GetTransform();
        this->SetWorldTransform( camera->GetWorldTransform() * mtx );

        this->GetRootComponent_NoLock()->SetPosition(
            this->GetRootComponent_NoLock()->GetPosition()
            + (this->GetRootComponent_NoLock()->GetForward() * 1.1 ) );



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
