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
    MeshComponent( MeshComponent&& source );
    virtual ~MeshComponent();

    MeshComponent& operator=( const MeshComponent& other ) = delete;
    MeshComponent& operator=( MeshComponent&& source );
    bool operator==( const MeshComponent& other ) const = delete;

    const std::string& GetMeshPath() const;
    void SetMeshPath( const std::string& str );

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
    std::string         mMeshName;
    engine::MeshPtr     mMesh;


};

} // end of namespace 'engine'
