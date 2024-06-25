#pragma once


#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

//namespace ClassNameSpace{

PT_FORWARD_DECLARE_CLASS( WorldAxisActor )

class WorldAxisActor: public engine::Actor
{
public:
    WorldAxisActor();
    WorldAxisActor( const std::string& name );

    WorldAxisActor( const WorldAxisActor& other );
    WorldAxisActor( WorldAxisActor&& source );
    virtual ~WorldAxisActor();
    WorldAxisActor& operator=( const WorldAxisActor& other );
    WorldAxisActor& operator=( WorldAxisActor&& source );

    bool operator==( const WorldAxisActor& other ) const;
protected:
    void OnTick(float t, float dt);
    void OnSpawned();
    void OnDespawned();
    bool OnCreateRenderContext();
    void OnDestroyRenderContext();
private:
    engine::AxisDisplayComponentPtr mAxisComponent;

};

//} // end of namespace 'ClassNameSpace'
