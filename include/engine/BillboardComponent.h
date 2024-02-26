#pragma once

#include "engine/RealComponent.h"
#include "engine/gl/Buffer.hpp"

namespace engine{

PT_FORWARD_DECLARE_CLASS( BillboardComponent )

class BillboardComponent: public RealComponent
{
public:
    BillboardComponent( const std::string& name );
    BillboardComponent( const BillboardComponent& other ) = delete;
    BillboardComponent( BillboardComponent&& source ) = delete;
    virtual ~BillboardComponent(){}
    BillboardComponent& operator=( const BillboardComponent& other ) = delete;
    BillboardComponent& operator=( BillboardComponent&& source ) = delete;

    bool operator==( const BillboardComponent& other ) const = delete;

    void OnDraw( float t, float dt ) override;

    void SetWidth( float width );
    void SetHeight( float height );
    void SetSize( float width, float height );

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    void OnTick( float t, float dt ) override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    void InitVertexData();

    float mWidth    = 2.0f; // [-1, 1]
    float mHeight   = 2.0f; // [-1, 1]
    gl::Buffer<gl::Vertex> mVertexBuffer;
};

} // end of namespace 'engine'
