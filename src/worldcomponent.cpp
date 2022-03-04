#include "worldcomponent.h"

#include "services.h"
#include "world.h"

#include <assert.h>
#include "pt/math.h"

using namespace pt;
using namespace engine;

float
calcAngle(const math::float3& vec)
{
    math::float3 x_axis(1.0f, 0.0f, 0.0f);
    float len = vec.length();
    if(0.0f == len){
        return 0.0f;
    }else{
        return acosf( x_axis.dot(vec) / vec.length() );
        //return acosf( x_axis.dot(vec) / (x_axis.length() * vec.length()) );
    }
}


math::float4x4
buildTransformMtx(math::float3 pos,
                  math::float4 orient,
                  math::float3 scale)
{
    //rotation mtx
    //| cos(f)  -sin(f)     0 |
    //| sin(f)   cos(f)     0 |
    //|   0         0       1 |

    //transformation mtx
    //| Sx * cos(f) Sx * -sin(f)    0       0 |
    //| Sy * sin(f) Sy *  cos(f)    0       0 |
    //|     0           0           1       0 |
    //|    Dx          Dy           0       1 |

    math::float3 xy_orient;
    xy_orient.x = orient.x * orient.w;
    xy_orient.y = orient.y * orient.w;
    xy_orient.z = 0.0f;
    float phi = calcAngle(xy_orient);

    //start with I mtx
    math::float4x4 mtx = math::float4x4::identity;
    mtx._00 = scale.x * cosf(phi);  mtx._01 = scale.x * sinf(phi) * (-1);
    mtx._10 = scale.y * sinf(phi);  mtx._11 = scale.y * cosf(phi);

    mtx._30 = pos.x;                mtx._31 = pos.y;

    return mtx;
}

//------------------------------------------------------------------------------


WorldComponent::
WorldComponent()
{}


WorldComponent::
~WorldComponent()
{}


bool WorldComponent::
operator==(const WorldComponent &other) const
{
    assert(false); //TODO: implement
    return true;
}


void WorldComponent::
spawn()
{
    World* world = Services::getWorld();
    world->spawnWorldComponent(this);
}


void WorldComponent::
setParent(WorldComponent *parent, bool bKeepPosition)
{
    mParent = parent;

    //check for cycles in parent hierarchy
    WorldComponent* current = this;
    while(nullptr == current->mParent){
        if(this == current->mParent){
            throw std::logic_error("WorldComponent parent hierarchy is not acyclic");
        }
        current = current->mParent;
    }

    //update position data
    refreshPosition(bKeepPosition);
}


void WorldComponent::
removeParent(bool bKeepPosition)
{
    mParent = nullptr;
    //update position data
    refreshPosition(bKeepPosition);
}


math::float3 WorldComponent::
getPosition() const
{
    return mPos;
}


math::float4 WorldComponent::
getOrientation() const
{
    return mOrient;
}


math::float3 WorldComponent::
getScale() const
{
    return mScale;
}


math::float4x4 WorldComponent::
getTransform() const
{
    return mTransform;
}


void WorldComponent::
setPosition(const math::float3 &pos)
{
    mPos = pos;
    refreshPosition();
}


void WorldComponent::
setOrientation(const math::float4 &orient)
{
    mOrient = orient;
    refreshPosition();
}


void WorldComponent::
setScale(const math::float3 &scale)
{
    mScale = scale;
    refreshPosition();
}


void WorldComponent::
setRelativeTransform(const math::float3 &pos,
                     const math::float4 &orient,
                     const math::float3 &scale)
{
    mPos = pos;
    mOrient = orient;
    mScale = scale;
    refreshPosition();
}


void WorldComponent::
RegisterWorldComponentParts(WorldComponent *component)
{
    Services::getWorld()->addWorldComponent(component);
}


void WorldComponent::
UnregisterWorldComponentParts(WorldComponent *component)
{
    Services::getWorld()->removeWorldComponent(component);
}


void WorldComponent::
refreshPosition(bool bBasedOnAbsolute)
{
    if(bBasedOnAbsolute){
        //change relative transform based on absolute
        if(mParent){
        //calculate transform relative to parent based on their current world transforms
            assert(false);
            //TODO: implement...
        }else{
        //calculate transform relative to world
            math::float4 pos(1.0f, 1.0f, 1.0f, 1.0f);
            math::float4 orient = math::float4::xUnit;
            math::float4 scale(1.0f, 1.0f, 1.0f, 1.0f);
            pos *= mTransform;
            scale *= mTransform;
            mPos = math::float3(pos.x * pos.w, pos.y * pos.w, pos.z * pos.w);
            mOrient = orient * mTransform;
            mScale = math::float3(scale.x * scale.w, scale.y * scale.w, scale.z * scale.w);
        }
    }else{
        //change absolute transform based on relative
        math::float4x4 tf;
        if(mParent){
            //calculate new absolute position relative to parent
            math::float4x4 tf_relative = buildTransformMtx(mPos, mOrient, mScale);
            math::float4x4 tf_parent = mParent->getTransform();
            tf = tf_parent * tf_relative;
        }else{
            //calculate new absolute position relative to world
            tf = buildTransformMtx(mPos, mOrient, mScale);
        }
        Services::getWorld()->updateWorldComponentTransform(this, tf);
    }
}


//TODO: cleanup

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//                         old implementation
//----------------------------------------------------------------------
//----------------------------------------------------------------------

/*

void WorldComponent::
        RegisterComponent(WorldComponent* component){
    WorldComponent::SyncProxy* proxy;
    proxy =  Services::instance()->
                            getWorld()->
                                addComponent(component);
    delete component->_proxy;
    component->_proxy = proxy;
    component->onRegister();
    ev_on_registered(this);
}

void WorldComponent::
        UnregisterComponent(WorldComponent* component){
    if(component->_proxy){
        component->_proxy->unregister();
    }
    delete component->_proxy;
    component->_proxy = nullptr;
    component->onUnregister();
    ev_on_unregistered(this);
}

WorldComponent* WorldComponent::
        getParent(){
    return _parent;
}

void WorldComponent::
        setParent(WorldComponent *parent, bool bKeepPosition){
    World* world = Services::instance()->getWorld();
    WorldComponent* parent_old = _parent;
    transform2d tf_rel_current = getTransformRelative();
    transform2d tf_world_current = getTransformWorld();

    //if new parent exists, but isn't registered
    if(parent){
        if(! world->isRegistered(parent)){
            throw std::logic_error("Tried to set unregistered parent");
        }
    }
    //---past here, parent is surely valid---

    //if already had a parent
    if(parent_old){
        //remove all listeners from current parent
        parent_old->ev_on_registered.remove_object(this);
        parent_old->ev_on_unregistered.remove_object(this);
        parent_old->ev_on_position_changed.remove_object(this);
    }

    //store new parent
    _parent = parent;

    //if new parent exists
    if( parent ){
        //register to parent's events
        parent->ev_on_registered.add(this, WorldComponent::onParentRegistered);
        parent->ev_on_unregistered.add(this, WorldComponent::onParentUnregistered);
        parent->ev_on_position_changed.add(this, WorldComponent::onParentPositionChanged);
    }

    //setup parent transform mtx or I if missing
    math::float4x4 m_parent = getParentWorldTransformMtx();

    //set coordinates based on new parent
    if(bKeepPosition){
        //TODO: implement
        assert(false);
    }else{
        transform2d tf_world_new = tf_rel_current * m_parent;
        _proxy->setComponentTransform( tf_world_new );
        //no good here
        //(multiple transforms inside the same world-update frame
        //  will cause multiple event calls during 1 World tick)

        //will this really be a problem?
        //  - children should move with parent immediately
        //  - outside world should only be notified on world.tick()
        ev_on_position_changed(tf_world_current, tf_world_new);
    }
}

void WorldComponent::
        removeParent(bool bKeepPosition){
    setParent(nullptr, bKeepPosition);
}

void WorldComponent::
        setTransformRelative(const transform2d& tf){
    transform2d tf_world_current = getTransformWorld();
    transform2d tf_world_new;

    //set new relative and world positions
    _transform_rel = tf;
    tf_world_new = tf * getParentWorldTransformMtx();
    // update listeners
    ev_on_position_changed(tf_world_current, tf_world_new);
}

void WorldComponent::
        setTransformWorld(const transform2d& tf){
    transform2d tf_rel_new;
    transform2d tf_world_parent_inverted;

    tf_world_parent_inverted = getParent()->getTransformWorld();
    //invert pos
    tf_world_parent_inverted._pos *= -1;
    //invert angle
    //      get angle
    math::float3 dir    = tf_world_parent_inverted.getOrient();
    math::float4 dirvec(dir,1);

    //may not have float3 * float4x4
    dir = dir * create2dRotationMtxZ( calcAngle(dir) );


    //invert scale
    tf_world_parent_inverted._scale = 1 / tf_world_parent_inverted.getScale();

    //need to invert the effects of the parent's transform
    //tf - tf_world_parent_inverted = tf_relative_new


}

void WorldComponent::
        worldcomponent_registered(Component *component){

}

void WorldComponent::
        onParentRegistered(WorldComponent *parent){
}

void WorldComponent::
        onParentUnregistered(WorldComponent *parent){
    removeParent(true);
}

void WorldComponent::
        onParentPositionChanged(transform2d &previous_pos,
                                transform2d &parent_pos){
    World* world = Services::instance()->getWorld();
    transform2d my_world_tf = parent_pos + _transform_rel;
    transform2d my_world_tf_old = ;

    //my_world_tf = parent_world_tf + my_rel_tf
    //setTransformWorld(my_world_tf);
    //ev_on_position_changed(my_world_tf_old, my_world_tf);
}

void WorldComponent::
        setTransformRelativeSilent(transform2d tf){

}

math::float4x4 WorldComponent::
        getParentWorldTransformMtx() const{
    if(parent){
        return _parent->getTransformWorld().getMtx();
    }else{
        return math::float4x4::identity;
    }
}

void WorldComponent::OnRegister_internal()
{

}

WorldComponent::
        WorldComponent(){

}

WorldComponent::
        WorldComponent(const WorldComponent &other){

}

WorldComponent::
        WorldComponent(WorldComponent &&other){

}

WorldComponent::
        ~WorldComponent(){
    //CHECK: should unregister here, or demand it, before delete?
}

WorldComponent &&WorldComponent::operator=(const WorldComponent &other)
{

}

bool WorldComponent::operator==(const WorldComponent &other) const
{

}

void WorldComponent::
        setPositionRelative(math::float3 pos){
    _transform_rel.setPos(pos);
}

void WorldComponent::
        setOrientationRelative(math::float3 orient){
    _transform_rel.setOrient(orient);
}

void WorldComponent::
        setScaleRelative(math::float3 scale){
    _transform_rel.setScale(scale);
}

void WorldComponent::
        setPositionWorld(math::float3 pos){
    //get requirements
    World* world = Services::instance()->getWorld();
    transform2d tran_world = world->getComponentTransform(this);
    transform2d tran_rela = _transform_rel;
    math::float3 posw = tran_world.getPos();
    math::float3 posr = tran_rela.getPos();

    //calculate new relative position
    math::float3 pos_delta = pos - posw;
    math::float3 newpos = posr + pos_delta;

    //set new position

    world->markComponentDirty(this);


}

void WorldComponent::
        setOrientationWorld(math::float3 orient){

}

void WorldComponent::
        setScaleWorld(math::float3 scale){

}

transform2d WorldComponent::
        getTransformRelative() const{
    return _transform_rel;
}

transform2d WorldComponent::
        getTransformWorld() const{
    return _proxy->getComponentTransform();
}

void WorldComponent::
        spawn(){
    _proxy->spawn();
    onSpawn();
}
*/
