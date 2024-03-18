#pragma once

#include "engine/RealComponent.h"

#include "engine/gl/Buffer.hpp"
#include "engine/DefaultShaderProgram.h"
#include "engine/gl/Texture.h"
#include "engine/gl/Uniforms.hpp"

namespace engine{

namespace exp{
struct FRotator{
    float mYaw = 0.0f;
    float mPitch = 0.0f;
    float mRoll = 0.0f;

    FRotator( float yaw, float pitch, float roll );
    FRotator( const math::float3& values );
    math::float4x4 GetTransform() const;
};
}

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

    gl::Texture2dPtr mTexture;
    float mWidth    = 2.0f; // [-1, 1]
    float mHeight   = 2.0f; // [-1, 1]
    gl::Buffer<gl::Vertex> mVertexBuffer;

    //origo
    gl::Buffer<gl::Vertex>      mOrigoVertexBuffer;
    gl::Buffer<unsigned int>    mOrigoIndexBuffer;

};

} // end of namespace 'engine'
