#pragma once

#include "engine/RealComponent.h"

#include "engine/gl/Buffer.hpp"
#include "engine/gl/Def.h"
#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( AxisDisplayComponent )

class AxisDisplayComponent: public RealComponent
{
public:
    AxisDisplayComponent();
    AxisDisplayComponent( const std::string& name );

    AxisDisplayComponent( const AxisDisplayComponent& other ) = delete;
    AxisDisplayComponent( AxisDisplayComponent&& source ) = delete;
    virtual ~AxisDisplayComponent();
    AxisDisplayComponent& operator=( const AxisDisplayComponent& other ) = delete;
    AxisDisplayComponent& operator=( AxisDisplayComponent&& source ) = delete;

    bool operator==( const AxisDisplayComponent& other ) const = delete;

protected:
    void OnDraw( float t, float dt ) override;
    void OnTick( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;
private:
    gl::Buffer<gl::Vertex>      mVertexBuffer;
    gl::Buffer<unsigned int>    mIndexBuffer;

    // Component interface
    protected:
    void OnSpawned() override;
    void OnDespawned() override;
    gl::DrawStage GetPreferredDrawStage() const override;

};

} // end of namespace 'engine'
