/** -----------------------------------------------------------------------------
  * FILE:    RealComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component with geometrical representation (visual or physical)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/experimental/WorldComponent.h"

#include "engine/experimental/Mesh.h"

#include <memory>

namespace engine{
namespace experimental{

class RealComponent;
using RealComponentPtr  = std::shared_ptr< RealComponent >;
using RealComponentPtrW = std::weak_ptr< RealComponent >;

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


    void OnAddedToEntity( ComponentVisitor& visitor ) override;
    void OnRemovedFromEntity( ComponentVisitor& visitor ) override;

    //void Tick( float t, float dt ) override;

    //void Decouple() override;

    const MeshPtr GetMesh();


    virtual void CreateContext();
    virtual void DestroyContext();

    virtual void Draw( float t, float dt ) = 0;

protected:
    //void OnSpawned() override;
    //void OnDespawned() override;

    virtual bool OnCreateContext() = 0;
    virtual bool OnDestroyContext() = 0;
private:
    bool    mContextInitialized = false; //here, or in children?
    MeshPtr mMesh;

};

} //end of namespace experimental
} //end of namespace engine
