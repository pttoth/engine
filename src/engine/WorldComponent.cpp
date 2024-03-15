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
BuildTransformMtx(const math::float3& pos,
                  const math::float4& orient,
                  const math::float3& scale)
{
    math::float4x4  mat_scale = math::float4x4::identity;
    mat_scale.m[0][0] *= scale.v[0];
    mat_scale.m[1][1] *= scale.v[1];
    mat_scale.m[2][2] *= scale.v[2];

    math::float3 dir = math::float3( orient.x, orient.y, orient.z );
    if( 0 != orient.w ){
        float w = orient.w;
        dir = math::float3( orient.x/w, orient.y/w, orient.z/w );
    }
    math::float4x4  mat_orient = CalcRotMtx( dir, math::float3(0.0f, 0.0f, 1.0f) );

    math::float4x4  mat_translation = math::float4x4::identity;
    mat_translation.m[0][3] -= pos.v[0];
    mat_translation.m[1][3] -= pos.v[1];
    mat_translation.m[2][3] -= pos.v[2];

    return mat_translation * mat_orient * mat_scale;
}


WorldComponent::
WorldComponent( const std::string &name ):
    Component( name ),
    EvOnTransformChanged( EvOnTransformChangedTrigger ),
    mOrient(1.0f, 0.0f, 0.0f, 1.0f),
    mScale(1.0f, 1.0f, 1.0f)
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


const math::float4 WorldComponent::
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
    std::string name = this->GetName();

    if(nullptr == mParent){
        return mTransform; //TODO: cache the worldTransforms in World and get the transform value from there
    }else{
        return mTransform * mParent->GetWorldTransform();
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
SetOrientation( const math::float4 &orient )
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
SetRelativeTransform( const math::float3 &pos,
                      const math::float4 &orient,
                      const math::float3 &scale )
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

    EvOnTransformChangedTrigger( this );
}

