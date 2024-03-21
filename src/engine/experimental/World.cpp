#include "engine/experimental/World.h"

#include "engine/Services.h"
#include "engine/component/WorldComponent.h"
#include "pt/utility.hpp"
#include <assert.h>
#include <exception>


using namespace engine;


void World::
addWorldComponent(WorldComponent *component)
{
    if( (!pt::ContainedInVector(mComponentsBuffered, component))
      &&(!pt::ContainedInVector(mComponentsSpawned, component)) ){
        mComponentsBuffered.push_back(component);
    }else{
        throw std::logic_error("tried to add already contained WorldComponent");
    }
}


void World::
removeWorldComponent(WorldComponent *component)
{
    //check spawned components
    int idx = pt::IndexOfInVector(mComponentsSpawned, component);
    if( -1 < idx ){
        pt::RemoveElementInVector(mComponentsSpawned, idx);
        return;
    }

    //check buffered components
    idx = pt::IndexOfInVector(mComponentsBuffered, component);
    if( -1 < idx ){
        pt::RemoveElementInVector(mComponentsBuffered, idx);
        return;
    }
}


void World::
addWorldComponent2(experimental::WorldComponent *component)
{
    if( (!pt::ContainedInVector( mComponentsBuffered2, component ))
      &&(!pt::ContainedInVector( mComponentsSpawned2, component )) ){
        mComponentsBuffered2.push_back( component );
    }else{
        throw std::logic_error("tried to add already contained WorldComponent");
    }
}


void World::
removeWorldComponent2(experimental::WorldComponent *component)
{
    //check spawned components
    int idx = pt::IndexOfInVector( mComponentsSpawned2, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mComponentsSpawned2, idx );
        return;
    }

    //check buffered components
    idx = pt::IndexOfInVector( mComponentsBuffered2, component );
    if( -1 < idx ){
        pt::RemoveElementInVector( mComponentsBuffered2, idx );
        return;
    }
}


void World::
SpawnWorldComponent(WorldComponent *component)
{
    //TODO: remove
    return;

    //check buffered components
    int idx = pt::IndexOfInVector(mComponentsBuffered, component);
    if( -1 < idx ){
        pt::RemoveElementInVector(mComponentsBuffered, idx);
        mComponentsSpawned.push_back(component);
    }else{
        throw std::logic_error("tried to spawn unregistered WorldComponent");
    }
}


void World::
DespawnWorldComponent(WorldComponent *component)
{
    assert(false); //TODO: implement
}


void World::
updateWorldComponentTransform(WorldComponent *component, math::float4x4 tf)
{
    //used later for optimized searches
    //TODO: implement...
}


//----------------------------------------------------------------------
//----------------------------------------------------------------------
//                              old header
//----------------------------------------------------------------------
//----------------------------------------------------------------------
/*
int World::
        indexOf(WorldComponent* const component) const{
    //assumes correct input
    for(size_t i=0; i<_contents.size(); ++i){
        if(_contents[i].component == component){
            if( INT_MAX < i){
                std::cerr << "CRITICAL ERROR: the number of World's contents exceeded INT_MAX!\n";
                assert(true); // amount of content exceeded INT_MAX
            }
            return i;
        }
    }
    return -1;
}

int World::
        indexOfDirty(WorldComponent * const component) const{
    //assumes correct input
    for(size_t i=0; i<_dirtyops.size(); ++i){
        if((_dirtyops[i].type != operation::NO_OPERATION)
         &&(_dirtyops[i].component == component)){
            if( INT_MAX < i){
                std::cerr << "CRITICAL ERROR: the number of World's dirtyops exceeded INT_MAX!\n";
                assert(true); // amount of content exceeded INT_MAX
            }
            return i;
        }
    }
    return -1;
}

transform2d World::
        determineComponentTransform(WorldComponent* const component) const{
    WorldComponent* parent;
    transform2d tf;

    //check invalid arguments
    assert(component);

    //get relevant component info
    tf = component->getTransformRelative();
    parent = component->getParent();

    //if parent doesn't exist, return the relative position
    if( !parent ){
        return tf;
    }
    //if parent exists, assume it is a valid and already registered component

    //get world coordinates of parent and build transformation matrix for it
    int idx = indexOf(parent);
    math::float4x4 mtx_parent_tf = _contents[idx].tf.getMtx();

    //return new transform2d with results of the parent-transformation
    return tf * mtx_parent_tf;
}

World::
    World(){

}

World::
    World(const World &other){

}

World::
    World(World &&other){

}

World::
    ~World(){

}

World& World::
        operator=(const World &other){
    //TODO: implement
    assert(false);
}

World& World::
        operator=(World &&other){
    //TODO: implement
    assert(false);
}

bool World::
        operator==(const World &other) const{
    //TODO: implement
    assert(false);
}

bool World::
        getLooped() const{
    return _bLooped;
}

void World::
        setLooped(bool val){
    _bLooped = val;
}

void World::processDirtyContents(float f, float dt)
{

}

void World::processPhysics(float t, float dt)
{

}

#ifdef WORLD_ENABLE_RESIZE
math::int3
        World::getSize() const{
    return _world_size;
}

void World::
        setSize(math::int3 size){
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _world_size = size;
    }
    onSizeChanged();
}

void World::
        onSizeChanged(){

    //TODO:
    //create inner thread
    //lock world
    //return
    //process changes
}
#endif



WorldComponent::SyncProxy* World::
        addWorldComponent(WorldComponent* component){
    if(!component){
        throw std::invalid_argument("tried to register nullptr as component");
    }
    {
        std::lock_guard<std::mutex> lock(_mutex);

        //if dirty (it is already in, or will be in on next tick)
        if( isDirty(component) ){
            throw std::logic_error("tried to add already registered component");
        }

        //if already contained
        if( isRegistered(component) ){
            throw std::logic_error("tried to add already registered component");
        }

        //if parent isn't contained
        if(!isRegistered( component->getParent() )){
            throw std::logic_error("tried to register component with unregistered parent");
        }

        //set up local component data
        obj_data data(&component,
                      determineComponentTransform(component) );

        //add local component data
        _contents.push_back(data);

        WorldComponent::SyncProxy* proxy = SyncObject(component);
        return proxy;
    }
}

transform2d World::
        getComponentTransform(WorldComponent * const component) const{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        int idx = indexOf(component);
        if(idx < 0){
            throw std::logic_error("Tried to access unregistered component");
        }

        return _contents[idx].tf;
    }
}

bool World::
        isRegistered(WorldComponent* const component) const{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(component){
            if( indexOf(component) >= 0){ return true; }
        }
        return false;
    }
}

bool World::
        isDirty(WorldComponent* const component) const{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if(component){
            if( indexOfDirty(component) >= 0){ return true; }
        }
        return false;
    }
}

void World::
        removeWorldComponent(WorldComponent* component){
    if(!component){
        throw std::invalid_argument("tried to unregister nullptr as component");
    }
    {
        std::lock_guard<std::mutex> lock(m);

        int idx = indexOf(component);

        //if it's registered
        if( idx >= 0){
            _contents[idx].invalidate();
        }
    }
}

void World::
        tickop_addcomponent(OpAdd op){

}

void World::
        tickop_removecomponent(OpRemove op){

}

void World::
        tickop_settransform(OpSetTransform op){

}

void World::
        setComponentTransform(WorldComponent* component, transform2d tf){
    if( !component){
        throw std::invalid_argument("tried to manipulate nullptr as component");
    }
    {
        std::lock_guard<std::mutex> lock(m);

        int idx = indexOf(component);

        //if it's registered
        if( idx < 0){
            throw std::logic_error("tried to access unregistered component");
        }
        //set new component transform
        _contents[idx].tf_state = tf;
    }
}

void World::
        spawnWorldComponent(WorldComponent *component){

}



World::SyncObject::
        SyncObject(WorldComponent *component){
    _component = component;
}

void World::SyncObject::
        setComponentTransform(transform2d tf){
    Services::getWorld()->setComponentTransform(_component, tf);
}

transform2d World::SyncObject::
        getComponentTransform(){
    return Services::getWorld()->getComponentTransform(_component);
}

void World::SyncObject::
        spawn(){
    Services::getWorld()->spawnWorldComponent(_component);
}

void World::SyncObject::
        unregister(){
    Services::getWorld()->removeWorldComponent(_component);
}
*/

