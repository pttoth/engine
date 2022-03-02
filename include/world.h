/** -----------------------------------------------------------------------------
  * FILE:    world.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Provides the environment representing the game space
  *             houses the physics system, so it can calculate subsequent states based on elapsed time
  * -----------------------------------------------------------------------------
  */

#pragma once

#include <mutex>

#include "pt/math.h"
#include "worldcomponent.h"
#include <vector>

//#define WORLD_ENABLE_RESIZE

namespace pt{
    struct transform2d; //TODO: check whether to remove
}
namespace engine{

class Entity;
class Component;

class World{
    std::vector<WorldComponent*> mComponentsBuffered;
    std::vector<WorldComponent*> mComponentsSpawned;
public:
    void addWorldComponent(WorldComponent* component);
    void removeWorldComponent(WorldComponent* component);
    void spawnWorldComponent(WorldComponent* component);
    void updateWorldComponentTransform(WorldComponent* component, pt::math::float4x4 tf);
};

}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//                              old header
//----------------------------------------------------------------------
//----------------------------------------------------------------------
/*
class Woerld{


    enum class operation{
        NO_OPERATION = 0,
        ADD_COMPONENT,
        REM_COMPONENT,
        SET_TRANSFORM,
    };

    struct OpAdd{
        operation op;
        WorldComponent* component;
    };
    struct OpRemove{
        operation op;
        WorldComponent* component;
    };
    struct OpSetTransform{
        operation op;
        WorldComponent* component;
        transform2d tf;
    };

    union dirtyop{
        struct{
            operation       type;
            WorldComponent* component;
        };
        OpAdd           add_component;
        OpRemove        remove_component;
        OpSetTransform  set_transform;
    };

public:  

    struct obj_data{
        WorldComponent* component; //id
        transform2d     tf;

        obj_data(WorldComponent* id,
                    transform2d  transform): component(id),
                                             tf(transform){
        }

        void invalidate(){
            component = nullptr;
        }
    };

    World();
    World(const World& other);
    World(World&& other);
    virtual ~World();
    World& operator=(const World &other);
    World& operator=(World &&other);
    bool operator==(const World &other)const;

    bool getLooped() const;
    void setLooped(bool val);

#ifdef WORLD_ENABLE_RESIZE
    math::int3 getSize() const;
    void setSize(math::int3 size);    //only needed if you want the map to loop
#endif

    void processDirtyContents(float f, float dt);
    void processPhysics(float t, float dt);

    //note: should this throw exceptions on repeated additions?
    //      - handle with macro?
    WorldComponent::SyncProxy* addComponent(WorldComponent* component); //throws logic_error, invalid_argument
    transform2d getComponentTransform(WorldComponent* const component) const;
    bool isRegistered(WorldComponent* const component) const;
    bool isDirty(WorldComponent* const component) const;
private:

//--------------------
//     variables
//--------------------
    bool       _bLooped;
    math::int3 _world_size;
    std::recursive_mutex  _mutex;
    std::vector<obj_data> _contents;
    std::vector<dirtyop> _dirtyops;


    int indexOf(WorldComponent* const component) const;
    int indexOfDirty(WorldComponent* const component) const;
    //assumes correct input (no check)
    transform2d determineComponentTransform(WorldComponent* const component) const;
    //std::vector<WorldComponent*> _dirty_components;
    //void markComponentDirty(WorldComponent& component, operation op);

    void setComponentTransform(WorldComponent* component, transform2d tf); //throws logic_error
    void spawnComponent(WorldComponent* component);
    void removeComponent(WorldComponent* component);

    void tickop_addcomponent(OpAdd op );
    void tickop_removecomponent(OpRemove op );
    void tickop_settransform(OpSetTransform op );

#ifdef WORLD_ENABLE_RESIZE
    void onSizeChanged();
#endif

     //
     //@brief SyncObject routes methods required for Component data manipulation
     //          to private World functions from outside
     //         It has full access to the private methods of World for manipulation.
     //
    class SyncObject: public WorldComponent::SyncProxy{
        WorldComponent* _component;
    public:
        SyncObject(WorldComponent* component);
        SyncObject(const SyncObject& other): _component(other._component){
        }

        virtual ~SyncObject(){}
        virtual void setComponentTransform(transform2d tf) override; //throws logic_error
        virtual transform2d getComponentTransform() override;
        virtual void spawn() override;
        virtual void unregister() override;
    };
};

} }
*/
