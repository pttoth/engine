#include "engine/component/WorldComponent.h"

#include "engine/Services.h"
#include "engine/Utility.h"
#include "engine/experimental/World.h"
#include "pt/math.h"
#include "pt/logging.h"
#include <assert.h>

using namespace engine;
using namespace math;
using namespace pt;

math::float4x4
BuildTransformMtx( const math::float3& pos,
                   const FRotator& rotation,
                   const math::float3& scale )
{
    math::float4x4  mat_scale = CalcScaleMtx( scale );
    math::float4x4  mat_orient = rotation.GetTransform();
    math::float4x4  mat_trans = CalcTranslationMtx( pos );
    return mat_trans * mat_orient * mat_scale;
}


math::float4x4
BuildTransformMtx( const math::float3& pos,
                   const math::float4x4& rotation,
                   const math::float3& scale )
{
    math::float4x4  mat_scale = CalcScaleMtx( scale );
    math::float4x4  mat_trans = CalcTranslationMtx( pos );
    return mat_trans * rotation * mat_scale;
}


WorldComponent::
WorldComponent( const std::string& name ):
    Component( name ),
    EvOnTransformChanged( EvOnTransformChangedTrigger ),
    mTransform( BuildTransformMtx( mPos, mRotMtx, mScale ) )
{}


WorldComponent::
WorldComponent( WorldComponent&& source ):
    Component( std::move( source ) ),
    EvOnTransformChangedTrigger( std::move( source.EvOnTransformChangedTrigger ) ),
    EvOnTransformChanged( EvOnTransformChangedTrigger )
{
    PT_UNIMPLEMENTED_FUNCTION
}


WorldComponent::
~WorldComponent()
{
    //TODO: design a decoupling and a destruction lifecycle step
    //  WorldComponent destructor steps may involve calling virtual functions of already destroyed instances
    //  decoupling should NOT be left to the destructor and should only be a failsafe
    RemoveParent();
    auto children = GetChildren();
    for(WorldComponent* wc : children){
        wc->RemoveParent();
    }
}


WorldComponent& WorldComponent::
operator=( WorldComponent&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
            return *this;
}


float3 WorldComponent::
ExtractPositionFromTransform( const math::float4x4& transform )
{
    const mat4& m = transform;
    return float3( m._03, m._13, m._23 );
}


float4x4 WorldComponent::
ExtractRotationFromTransform( const math::float4x4& transform )
{
    mat4 retval;
    DissectTransform( nullptr, &retval, nullptr, transform );
    return retval;
}


float3 WorldComponent::
ExtractScaleFromTransform( const math::float4x4& transform )
{
    const mat4& m = transform;
    float Sx = float3( m._00, m._10, m._20 ).length();
    float Sy = float3( m._01, m._11, m._21 ).length();
    float Sz = float3( m._02, m._12, m._22 ).length();

    if( Sx * Sy * Sz <= 0.0f ){
        PT_LOG_LIMITED_WARN( 10, "WorldComponent::ExtractScaleFromTransform() returns invalid values if they would be negative! TODO: implement correctly!" );
    }

    return float3( Sx, Sy, Sz );
}


void WorldComponent::
DissectTransform( math::float3* position, math::float4x4* rotation, math::float3* scale, const math::float4x4& transform )
{
    if( nullptr != rotation ){
        const mat4& m = transform;
              mat4& r = *rotation;

        math::float3 lScale = ExtractScaleFromTransform( transform );

        float& Sx = lScale.x;
        float& Sy = lScale.y;
        float& Sz = lScale.z;

        // Calculate rotation
        // WARNING: only works, when scale is positive!
        // TODO:    implement correctly
        //          see: https://math.stackexchange.com/a/1463487
        //               https://math.stackexchange.com/a/3554913
        // TODO:    remove log warning in 'WorldComponent::ExtractScaleFromTransform()'
        r = mat4::identity;
        r._00 = m._00/Sx;       r._01 = m._01/Sy;       r._02 = m._02/Sz;
        r._10 = m._10/Sx;       r._11 = m._11/Sy;       r._12 = m._12/Sz;
        r._20 = m._20/Sx;       r._21 = m._21/Sy;       r._22 = m._22/Sz;
        if( nullptr != position ){
            *position = ExtractPositionFromTransform( transform );
        }
        if( nullptr != scale ){
            *scale    = lScale;
        }
    }else{
        if( nullptr != position ){
            *position = ExtractPositionFromTransform( transform );
        }
        if( nullptr != scale ){
            *scale    = ExtractScaleFromTransform( transform );
        }
    }
}


float3 WorldComponent::
GetForward() const
{
    return ( GetRotationMtx() * vec4::xUnit ).XYZ();
}


float3 WorldComponent::
GetBackward() const
{
    return GetForward() * -1;
}


float3 WorldComponent::
GetRight() const
{
    return ( GetRotationMtx() * (vec4::yUnit * -1) ).XYZ();
}


float3 WorldComponent::
GetLeft() const
{
    return GetRight() * -1;
}


float3 WorldComponent::
GetUp() const
{
    return ( GetRotationMtx() * vec4::zUnit ).XYZ();
}


float3 WorldComponent::
GetDown() const
{
    return GetUp() * -1;
}


void WorldComponent::
SetParent( WorldComponent* parent )
{
    if( mParent != nullptr ){
        mParent->RemoveChild( this );
    }

    //check for cycles in parent hierarchy
    if( parent != nullptr ){
        if( this == parent ){
            PT_LOG_ERR( GetName() << ": Tried to set self as parent. Skipping." );
            #ifdef PT_DEBUG_ENABLED
                pt::PrintStackTrace();
            #endif
        }
        WorldComponent* current = parent;
        while( nullptr != current->mParent ){
            if( this == current->mParent ){
                PT_LOG_ERR( GetName() << ": Failed to set '" << parent->GetName() << "' as parent, because it would break acyclic hierarchy!" );
                #ifdef PT_DEBUG_ENABLED
                    pt::PrintStackTrace();
                #endif
            }
            current = current->mParent;
        }
    }

    mParent = parent;

    if( nullptr != mParent ){
        mParent->AddChild( this );
    }
}


void WorldComponent::
RemoveParent()
{
    SetParent( nullptr );
}


WorldComponent *WorldComponent::
GetParent()
{
    return mParent;
}


std::vector<WorldComponent *> WorldComponent::
GetChildren()
{
    PT_LOG_LIMITED_WARN( 10, "WorldComponent::GetChildren() allocates vector per call! Need refactoring!" );
    std::vector< WorldComponent* > retval;
    retval.reserve( mChildren.size() );
    for( WorldComponent* wc : mChildren ){
        if( nullptr != wc ){
            retval.push_back( wc );
        }
    }
    return retval;
}


const math::float3 WorldComponent::
GetPosition() const
{
    return mPos;
}

/*
const math::float4 WorldComponent::
GetOrientation() const
{
    return mOrient;
}
*/

const float4x4 WorldComponent::
GetRotationMtx() const
{
    return mRotMtx;
}


const math::float3 WorldComponent::
GetScale() const
{
    return mScale;
}


const math::float4x4 WorldComponent::
GetRelativeTransform() const
{
    RefreshTransform();
    return mTransform;
}


const math::float3 WorldComponent::
GetWorldPosition() const
{
    if( nullptr == mParent ){
        return mPos;
    }else{
        return ExtractPositionFromTransform( this->GetWorldTransform() );
    }
}

/*
const math::float4 WorldComponent::
GetWorldOrientation() const
{
    PT_UNIMPLEMENTED_FUNCTION
    return math::float4();
}


const float4x4 WorldComponent::
GetWorldRotationMtx() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


const math::float3 WorldComponent::
GetWorldScale() const
{
    PT_UNIMPLEMENTED_FUNCTION
    // scale is currently not refreshed correctly, only transform
    if(nullptr == mParent){
        return mScale; //TODO: cache the worldTransforms in World and get the transform value from there
    }else{
        return mParent->GetWorldScale() * mScale;
    }
}
*/

const math::float4x4 WorldComponent::
GetWorldTransform() const
{
    if( nullptr == mParent ){
        return this->GetRelativeTransform();
    }else{
        return mParent->GetWorldTransform() * this->GetRelativeTransform();
    }
}


void WorldComponent::
SetPosition( const math::float3 &pos )
{
    mPos = pos;
    mTransformDirty = true;
}


void WorldComponent::
SetRotation( const math::FRotator& rotator )
{
    SetRotation( rotator.GetTransform() );
    mTransformDirty = true;
}


void WorldComponent::
SetRotation( const math::float4x4& rotation )
{
    mRotMtx = rotation;
    mTransformDirty = true;
}


void WorldComponent::
SetScale( const math::float3 &scale )
{
    if( scale.x <= 0.0f || scale.y <= 0.0f || scale.y <= 0.0f ){
        PT_LOG_LIMITED_ERR( 10, "Tried to set non-positive scale to WorldComponent '" << GetName() << "'. Transform is not yet handled in these cases!" );
        #ifdef PT_DEBUG_ENABLED
            static bool firsttime = true;
            if( firsttime ){
                firsttime = false;
                pt::PrintStackTrace();
            }
        #endif
    }
    mScale = scale;
    mTransformDirty = true;
}


void WorldComponent::
SetRelativeTransform( const math::float3& pos,
                      const math::FRotator& rotation,
                      const math::float3& scale )
{
    mPos = pos;
    //mOrient = orient;
    mRotMtx = rotation.GetTransform();
    mScale = scale;
    mTransformDirty = true;
}


void WorldComponent::
SetRelativeTransform( const math::float4x4& transform )
{
    DissectTransform( &mPos, &mRotMtx, &mScale, transform );
    mTransform = transform;
}


void WorldComponent::
SetWorldPosition( const math::float3& pos )
{
    if( nullptr == mParent ){
        SetPosition( pos );
    }else{
        // get parent's worldtransform and "subtract" it with an inverse multiplication
        mat4 tfParent = mParent->GetWorldTransform();
        SetPosition( pos - ExtractPositionFromTransform( tfParent ) );
    }
}

/*
void WorldComponent::
SetWorldOrientation( const math::float4& orient )
{

}
*/

void WorldComponent::
SetWorldRotation( const math::FRotator& rotator )
{
    if( nullptr == mParent ){
        this->SetRotation( rotator );
    }else{
        mat4 tfParentInv = mParent->GetWorldTransform().invert();
        mat4 tfLocal = tfParentInv * this->GetWorldTransform();

        float3  pos;
        float3  scale;
        DissectTransform( &pos, nullptr, &scale, tfLocal );
        this->SetPosition( pos );
        this->SetRotation( rotator.GetTransform() );
        this->SetScale( scale );
    }
}

/*
void WorldComponent::
SetWorldScale( const math::float3& scale )
{

}
*/

void WorldComponent::
SetWorldTransform( const math::float4x4& transform )
{
    if( nullptr == mParent ){
        SetRelativeTransform( transform );
    }else{
        // get parent's worldtransform and "subtract" it with an inverse multiplication
        mat4 tfParentInv = mParent->GetWorldTransform().invert();
        mat4 tfLocal = tfParentInv * transform;
        SetRelativeTransform( tfLocal );
    }
}

// --------------------------------------------------
//  protected
// --------------------------------------------------

void WorldComponent::
OnSpawned()
{
    //TODO: remove or re-enable
    //World* world = Services::GetWorld();
    //world->SpawnWorldComponent( this );
}


void WorldComponent::
OnDespawned()
{
    //TODO: remove or re-enable
    //World* world = Services::GetWorld();
    //world->DespawnWorldComponent( this );
}


void WorldComponent::
RefreshTransform() const
{
    if( mTransformDirty ){
        mTransform = BuildTransformMtx( mPos, mRotMtx, mScale );
        mTransformDirty = false;
    }
}

// --------------------------------------------------
//  private
// --------------------------------------------------

void WorldComponent::
AddChild( WorldComponent* component )
{
    if( nullptr == component ){
        PT_LOG_ERR( "Tried to add 'nullptr' as child component to '" << GetName() << "'" );
        assert( nullptr != component );
        return;
    }

    int idx = pt::IndexOfInVector( mChildren, component );
    assert( idx < 0 );
    if( -1 < idx ){
        PT_LOG_ERR( "Tried to add WorldComponent '" << component->GetName() << "' as a child to '" << GetName() << "', that already contains it!" );
    }else{
        mChildren.push_back( component );
    }
}


void WorldComponent::
RemoveChild( WorldComponent* component )
{
    if( nullptr == component ){
        PT_LOG_ERR( "Tried to remove 'nullptr' as child component from '" << GetName() << "'" );
        assert( nullptr != component );
        return;
    }

    int idx = pt::IndexOfInVector( mChildren, component );
    assert( -1 < idx );
    if( idx < 0 ){
        PT_LOG_ERR( "Tried to remove child WorldComponent'" << component->GetName() << "' from '" << GetName() << "', that does not contain it!" );
    }else{
        mChildren[idx] = nullptr;
    }
}


