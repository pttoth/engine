#include "engine/experimental/WorldComponent.h"

#include "engine/experimental/ComponentVisitor.h"

#include "pt/math.h"
#include "pt/logging.h"

#include <assert.h>

using namespace pt;
using namespace engine;
using namespace engine::experimental;


math::float4x4
BuildTransformMtx(const math::float3& pos,
                  const math::float4& orient,
                  const math::float3& scale)
{
    using namespace pt::math;

    //TODO: make it 3D!

    //rotation mtx
    //| cos(f)  -sin(f)     0 |
    //| sin(f)   cos(f)     0 |
    //|   0         0       1 |

    //transformation mtx
    //| Sx *  cos(f)  Sy * sin(f)    0       Dx  |
    //| Sx * -sin(f)  Sy * cos(f)    0       Dy  |
    //|     0             0          1       0   |
    //|     0             0          0       1   |

    float3 xy_orient;
    xy_orient.x = orient.x * orient.w;
    xy_orient.y = orient.y * orient.w;
    xy_orient.z = 0.0f;
    float phi = pt::math::CalcAngle(xy_orient, float3::xUnit);

    //start with I mtx
    float4x4 mtx = float4x4::identity;
    const float3& S = scale;
    const float3& D = pos;
    mtx._00 = S.x * cosf(phi);      mtx._01 = S.y * sin(phi);   mtx._03 = D.x;
    mtx._10 = S.x * (-1)*sin(phi);  mtx._11 = S.y * cos(phi);   mtx._13 = D.y;

    return mtx;
}


WorldComponent::
WorldComponent( const std::string &name ):
    Component( name ),
    mOrient(1.0f, 0.0f, 0.0f, 1.0f),
    mScale(1.0f, 1.0f, 1.0f)
{}


WorldComponent::
WorldComponent( const WorldComponent& other ):
    Component( other ),
    mPos( other.mPos ),
    mOrient( other.mOrient ),
    mScale( other.mScale ),
    mTransform( other.mTransform )
{}


WorldComponent::
WorldComponent( WorldComponent&& source ):
    Component( source ),
    mPos( source.mPos ),
    mOrient( source.mOrient ),
    mScale( source.mScale ),
    mTransform( source.mTransform )
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
OnAddedToEntity( ComponentVisitor &visitor )
{
    Component::OnAddedToEntity( visitor );
    visitor.AddWorldComponent();
}


void WorldComponent::
OnRemovedFromEntity( ComponentVisitor &visitor )
{
    Component::OnRemovedFromEntity( visitor );
    visitor.RemoveWorldComponent();
}


void WorldComponent::
Spawn()
{
    //TODO: remove or re-enable
    //World* world = Services::GetWorld();
    //world->SpawnWorldComponent( this );

    Component::Spawn();
}


void WorldComponent::
Despawn()
{
    //TODO: remove or re-enable
    //World* world = Services::GetWorld();
    //world->DespawnWorldComponent( this );

    Component::Despawn();
}

// TODO:        !!! thread-safety !!!
/*
void WorldComponent::
SetParent( WorldComponent *parent )
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
        mParent->AddChild(this);
    }

    //update position data
    RefreshTransform();
}


void WorldComponent::
RemoveParent()
{
    SetParent(nullptr);
}


WorldComponent *WorldComponent::
GetParent()
{
    return mParent;
}
*/


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

