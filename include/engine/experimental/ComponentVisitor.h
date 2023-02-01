//--------------------------------------------------
/** @brief ComponentVisitor class:
 *          Used to identify Component subclasses, when they are added (/removed) to (/from) an Entity
 */


//--------------------------------------------------


#pragma once

namespace engine{
namespace experimental{

class Actor;
class Component;

class ComponentVisitor
{
    Actor*      mActor;
    Component*  mComponent;

public:
    ComponentVisitor( Actor& actor, Component& component ):
        mActor( &actor ), mComponent( &component )
    {}
    ComponentVisitor( const ComponentVisitor& other ) = default;
    ComponentVisitor( ComponentVisitor&& other ) = default;
    virtual ~ComponentVisitor()
    {}
    ComponentVisitor& operator=( const ComponentVisitor &other ) = default;
    ComponentVisitor& operator=( ComponentVisitor &&other ) = default;
    bool operator==( const ComponentVisitor &other ) const = delete;

    void AddWorldComponent();
    void RemoveWorldComponent();
};

} //end of namespace experimental
} //end of namespace engine
