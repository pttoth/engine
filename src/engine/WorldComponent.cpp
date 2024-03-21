#include "engine/WorldComponent.h"

#include "engine/Services.h"
#include "engine/Utility.h"
#include "engine/World.h"

#include "pt/math.h"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace math;
using namespace pt;

math::float4x4
BuildTransformMtx( const math::float3& pos,
                   const FRotator& orientation,
                   const math::float3& scale )
{
    math::float4x4  mat_scale = CalcScaleMtx( scale );
    math::float4x4  mat_orient = orientation.GetTransform();
    math::float4x4  mat_trans = CalcTranslationMtx( pos );
    return mat_trans * mat_orient * mat_scale;
}


WorldComponent::
WorldComponent( const std::string& name ):
    Component( name ),
    EvOnTransformChanged( EvOnTransformChangedTrigger ),
    mTransform( BuildTransformMtx( mPos, mOrient, mScale ) )
{}


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


void WorldComponent::
Decouple()
{
    //TODO: decouple parent-child connections
    assert( false );
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
            pt::log::err << this->GetName() << ": Failed to set self as parent!";
            assert( false );
        }
        WorldComponent* current = parent;
        while( nullptr != current->mParent ){
            if( this == current->mParent ){
                pt::log::err << this->GetName() << ": Failed to set '" << parent->GetName() << "' as parent! Hierarchy would be not acyclic!";
                assert( false );
            }
            current = current->mParent;
        }
    }

    mParent = parent;

    if( nullptr != mParent ){
        mParent->AddChild( this );
        RefreshTransform();
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


const math::FRotator WorldComponent::
GetOrientation() const
{
    return mOrient;
}


const math::float3 WorldComponent::
GetScale() const
{
    return mScale;
}


const math::float4x4 WorldComponent::
GetTransform() const
{
    return mTransform;
}


const math::float3 WorldComponent::
GetWorldPosition() const
{
    assert( false );
    return math::float3();
}


const math::float4 WorldComponent::
GetWorldOrientation() const
{
    assert( false );
    return math::float4();
}


const math::float3 WorldComponent::
GetWorldScale() const
{
    assert( false );
    return math::float3();
}


const math::float4x4 WorldComponent::
GetWorldTransform() const
{
    if(nullptr == mParent){
        return mTransform; //TODO: cache the worldTransforms in World and get the transform value from there
    }else{
        return mParent->GetWorldTransform() * mTransform;
    }
}


void WorldComponent::
SetPosition( const math::float3 &pos )
{
    mPos = pos;
    //TODO: mark as dirty instead and recalc, when a dirty transform is read
    RefreshTransform();
}


void WorldComponent::
SetOrientation( const math::FRotator& orient )
{
    mOrient = orient;
    //TODO: mark as dirty instead and recalc, when a dirty transform is read
    RefreshTransform();
}


void WorldComponent::
SetScale( const math::float3 &scale )
{
    mScale = scale;
    //TODO: mark as dirty instead and recalc, when a dirty transform is read
    RefreshTransform();
}


void WorldComponent::
SetRelativeTransform( const math::float3& pos,
                      const math::FRotator& orient,
                      const math::float3& scale )
{
    mPos = pos;
    mOrient = orient;
    mScale = scale;
    //TODO: mark as dirty instead and recalc, when a dirty transform is read
    RefreshTransform();
}


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
AddChild( WorldComponent *component )
{
    assert( nullptr != component ); // TODO: add correct null-check
    int idx = pt::IndexOfInVector( mChildren, component );
    assert( idx < 0 );
    if( -1 < idx ){
        pt::log::err << "Tried to add WorldComponent '" << component->GetName()
                     << "' as a child to '" << this->GetName()
                     << "', that already contains it!\n";
    }else{
        mChildren.push_back( component );
    }
}


void WorldComponent::
RemoveChild( WorldComponent *component )
{
    assert( nullptr != component ); // TODO: add correct null-check
    int idx = pt::IndexOfInVector( mChildren, component );
    assert( -1 < idx );
    if( idx < 0 ){
        pt::log::err << "Tried to remove child WorldComponent'" << component->GetName()
                     << "' from '" << this->GetName()
                     << "', that does not contain it!\n";
    }else{
        mChildren[idx] = nullptr;
    }
}


void WorldComponent::
RefreshTransform()
{
    mTransform = BuildTransformMtx(mPos, mOrient, mScale);
    //change absolute transform based on relative

    // TODO: notify World of global position change
    if( mParent ){
        //calculate new absolute position relative to parent
        //math::float4x4 tf_parent = mParent->GetTransform();
        //Services::GetWorld()->updateWorldComponentTransform( this, mTransform * tf_parent );
    }else{
        //calculate new absolute position relative to world
        //Services::GetWorld()->updateWorldComponentTransform( this, mTransform );
    }


    //update children
    auto children = GetChildren(); //TODO: avoid per-frame memory allocation
    for( auto c : children ){
        c->RefreshTransform();
    }

    //fire event, notifying of transform change
    //EvOnTransformChangedTrigger( this );
}

