/** -----------------------------------------------------------------------------
  * FILE:    MeshComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Draws a mesh in 3d space.
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/component/RealComponent.h"

#include "engine/gl/Mesh.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( MeshComponent )

class MeshComponent: public RealComponent
{
public:
    MeshComponent( const std::string& name );
    MeshComponent( const MeshComponent& other ) = delete;
    MeshComponent( MeshComponent&& source ) = delete;
    virtual ~MeshComponent();

    MeshComponent& operator=( const MeshComponent& other ) = delete;
    MeshComponent& operator=( MeshComponent&& source ) = delete;
    bool operator==( const MeshComponent& other ) const = delete;

    void SetMesh( const std::string& mesh_name );

    void    SetUVScale( float scale );          // @TODO: rename + refactor with vec2, +getter, etc.
    void    EnableFixUVScale( bool enabled );   // @TODO: rename

protected:
    // Component interface
    void OnSpawned() override;
    void OnDespawned() override;
    void OnTick( float t, float dt ) override;

    // RealComponent interface
    void OnDraw( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    std::string     mMeshName;
    gl::MeshPtr     mMesh;

    bool    mFixUVScaleEnabled  = false;
    float   mFixUVScale         = 1.0f;
};

} // end of namespace 'engine'
