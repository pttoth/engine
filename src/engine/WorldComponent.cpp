#include "engine/WorldComponent.h"

#include "engine/Entity.h"
#include "engine/Services.h"
#include "engine/World.h"

#include "pt/math.h"
#include "pt/logging.h"

#include <assert.h>


using namespace pt;
using namespace engine;


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

//------------------------------------------------------------------------------


WorldComponent::
WorldComponent(const std::string& name):
    Component(name)
{
    mOrient = math::float4(1.0f, 0.0f, 0.0f, 1.0f);
    mScale = math::float3(1.0f, 1.0f, 1.0f);
}


WorldComponent::
WorldComponent(const WorldComponent &other):
    Component(other)
{
    mPos        = other.mPos;
    mOrient     = other.mOrient;
    mScale      = other.mScale;
    mTransform  = other.mTransform;
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


bool WorldComponent::
operator==(const WorldComponent &other) const
{
    assert(false); //TODO: implement
    return true;
}


void WorldComponent::
OnAddedToEntity(entity::ComponentVisitor& visitor)
{
    Component::OnAddedToEntity(visitor);
    visitor.AddWorldComponent();
}


void WorldComponent::
OnRemovedFromEntity(entity::ComponentVisitor& visitor)
{
    Component::OnRemovedFromEntity(visitor);
    visitor.RemoveWorldComponent();
}


void WorldComponent::
Spawn()
{
    World* world = Services::GetWorld();
    world->SpawnWorldComponent( this );

    Component::Spawn();
}


void WorldComponent::
Despawn()
{
    World* world = Services::GetWorld();
    world->DespawnWorldComponent( this );

    Component::Despawn();
}


void WorldComponent::
SetParent(WorldComponent *parent)
{
    if(mParent != nullptr){
        mParent->RemoveChild(this);
    }

    //check for cycles in parent hierarchy
    if(parent != nullptr){
        if(this == parent){
            pt::log::err << this->GetName() << ": Failed to set self as parent!";
            assert(false);
        }
        WorldComponent* current = parent;
        while(nullptr != current->mParent){
            if(this == current->mParent){
                pt::log::err << this->GetName() << ": Failed to set '" << parent->GetName() << "' as parent! Hierarchy would be not acyclic!";
                assert(false);
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
    assert(false); //TODO: implement
    return pt::math::float3();
}


const math::float4x4 WorldComponent::
GetWorldTransform() const
{
    using namespace pt::math;

    std::string name = this->GetName();

    if(nullptr == mParent){
        return mTransform; //TODO: cache the worldTransforms in World and get the transform value from there
    }else{
        return mTransform * mParent->GetWorldTransform();
    }
}


void WorldComponent::
SetPosition(const math::float3& pos)
{
    mPos = pos;
    RefreshTransform();
}


void WorldComponent::
SetOrientation(const math::float4& orient)
{
    mOrient = orient;
    RefreshTransform();
}


void WorldComponent::
SetScale(const math::float3& scale)
{
    mScale = scale;
    RefreshTransform();
}


void WorldComponent::
SetRelativeTransform(const math::float3& pos,
                     const math::float4& orient,
                     const math::float3& scale)
{
    mPos = pos;
    mOrient = orient;
    mScale = scale;
    RefreshTransform();
}


void WorldComponent::
AddChild(WorldComponent *component)
{
    int idx = pt::IndexOfInVector(mChildren, component);
    assert(idx < 0);
    if(-1 < idx){
        pt::log::err << "Tried to add WorldComponent '" << component->GetName() << "' as a child to '" << this->GetName() << "', that already contains it!\n";
    }else{
        mChildren.push_back(component);
    }
}


void WorldComponent::
RemoveChild(WorldComponent *component)
{
    int idx = pt::IndexOfInVector(mChildren, component);
    assert(-1 < idx);
    if(idx < 0){
        pt::log::err << "Tried to remove WorldComponent child '" << component->GetName() << "' from '" << this->GetName() << "', that does not contain it!\n";
    }else{
        mChildren[idx] = nullptr;
    }
}


std::vector<WorldComponent *> WorldComponent::
GetChildren()
{
    std::vector<WorldComponent*> retval;
    retval.reserve( mChildren.size() );
    for(WorldComponent* wc : mChildren){
        if(nullptr != wc){
            retval.push_back(wc);
        }
    }
    return std::move(retval);
}


void WorldComponent::
RefreshTransform()
{
    mTransform = BuildTransformMtx(mPos, mOrient, mScale);
    //change absolute transform based on relative
    if(mParent){
        //calculate new absolute position relative to parent
        math::float4x4 tf_parent = mParent->GetTransform();
        Services::GetWorld()->updateWorldComponentTransform(this, mTransform * tf_parent);
    }else{
        //calculate new absolute position relative to world
        Services::GetWorld()->updateWorldComponentTransform(this, mTransform);
    }


    //update children
    auto children = GetChildren(); //TODO: avoid per-frame memory allocation
    for(auto c : children){
        c->RefreshTransform();
    }
}
