#pragma once

#include "engine/component/RealComponent.h"

#include "engine/gl/Buffer.hpp"
#include "engine/DefaultShaderProgram.h"
#include "engine/gl/Texture2d.h"
#include "engine/gl/Uniform.hpp"

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

    // OpenGL context has to exist, when called.
    static bool Initialize();

    void EnableMonochrome( bool enabled );
    void EnableMonochrome( bool enabled, const math::float3& color );

    void SetWidth( float width );
    void SetHeight( float height );
    void SetSize( float width, float height );
    void SetTexture( gl::Texture2dPtr texture );

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    void OnTick( float t, float dt ) override;
    void OnDraw( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    void InitVertexData();

    static bool stInitialized;
    static gl::Buffer<unsigned int> stIndexBuffer;
    static const pt::Name stNameM;

    bool mUseColor = false;
    math::float3 mMonoColor = math::float3::white;
    gl::Texture2dPtr mTexture;
    float mWidth    = 2.0f; // [-1, 1]
    float mHeight   = 2.0f; // [-1, 1]
    gl::Buffer<gl::Vertex> mVertexBuffer;


};

} // end of namespace 'engine'
