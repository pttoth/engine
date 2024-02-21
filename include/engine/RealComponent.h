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

#include <memory>

namespace engine{

class RealComponent;
using RealComponentPtr       = std::shared_ptr< RealComponent >;
using ConstRealComponentPtr  = std::shared_ptr< const RealComponent >;
using RealComponentWPtr      = std::weak_ptr< RealComponent >;
using ConstRealComponentWPtr = std::weak_ptr< const RealComponent >;
using RealComponentUPtr      = std::unique_ptr< RealComponent >;
using ConstRealComponentUPtr = std::unique_ptr< const RealComponent >;

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


    virtual void CreateContext();
    virtual void DestroyContext();

    virtual void Draw( float t, float dt ) = 0;

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
