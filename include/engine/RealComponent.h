/** -----------------------------------------------------------------------------
  * FILE:    RealComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component with geometrical representation (for visuals or for physics)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/WorldComponent.h"
#include "engine/Mesh.h"
#include "pt/macros.h"
#include <memory>

namespace engine{

PT_FORWARD_DECLARE_CLASS( RealComponent )

class RealComponent: public WorldComponent
{
public:
    RealComponent( const std::string& name );
    RealComponent( const RealComponent& other ) = delete;
    RealComponent( RealComponent&& source ) = delete;

    virtual ~RealComponent();

    RealComponent& operator=( const RealComponent& other ) = delete;
    RealComponent& operator=( RealComponent&& source ) = delete;

    bool operator==( const RealComponent& other ) const = delete;


    //void Decouple() override;

    const MeshPtr GetMesh();

    void Spawn() override;
    void Despawn() override;

    virtual void CreateContext();
    virtual void DestroyContext();

    virtual void Draw( float t, float dt );
    virtual void OnDraw( float t, float dt ) = 0;

protected:
    //void OnSpawned() override;
    //void OnDespawned() override;
    //void OnTick( float t, float dt ) override;

    virtual bool OnCreateContext() = 0;
    virtual bool OnDestroyContext() = 0;
private:
    bool    mContextInitialized = false; //here, or in children?
    MeshPtr mMesh;

};

} //end of namespace engine
