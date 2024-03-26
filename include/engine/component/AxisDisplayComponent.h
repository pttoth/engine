#pragma once

#include "RealComponent.h"

#include "engine/gl/Buffer.hpp"
#include "engine/gl/Def.h"
#include "engine/Def.h"
#include "pt/macros.h"

// TODO: make vertices and indices static
//  axis size is only controlled by the scale property

namespace engine{

PT_FORWARD_DECLARE_CLASS( AxisDisplayComponent )

class AxisDisplayComponent: public RealComponent
{
public:
    AxisDisplayComponent();
    AxisDisplayComponent( const std::string& name );

    AxisDisplayComponent( const AxisDisplayComponent& other ) = delete;
    AxisDisplayComponent( AxisDisplayComponent&& source ) = delete; // TODO: enable
    virtual ~AxisDisplayComponent();
    AxisDisplayComponent& operator=( const AxisDisplayComponent& other ) = delete;
    AxisDisplayComponent& operator=( AxisDisplayComponent&& source ) = delete; // TODO: enable

    bool operator==( const AxisDisplayComponent& other ) const = delete;

protected:
    // Component interface
    void OnSpawned() override;
    void OnDespawned() override;
    void OnTick( float t, float dt ) override;

    // RealComponent interface
    gl::DrawStage GetPreferredDrawStage() const override;
    void OnDraw( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    gl::Buffer<gl::Vertex>      mVertexBuffer;
    gl::Buffer<unsigned int>    mIndexBuffer;

};

} // end of namespace 'engine'
