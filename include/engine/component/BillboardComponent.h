#pragma once

#include "RealComponent.h"

#include "engine/StandardShaderProgram.h"
#include "engine/gl/Buffer.hpp"
#include "engine/gl/Texture2d.h"
#include "engine/gl/Uniform.hpp"

// @TODO: rewrite parts where static data is used by the class
// @TODO: rewrite to draw with 'GL_TRIANGLE_STRIP' instead of indexed draw

//@TODO: add normal vectors to vertex data (doesn't receive light)

namespace engine{

PT_FORWARD_DECLARE_CLASS( BillboardComponent )

class BillboardComponent: public RealComponent
{
public:
    BillboardComponent( const std::string& name );
    BillboardComponent( const BillboardComponent& other ) = delete;
    BillboardComponent( BillboardComponent&& source ) = delete; // TODO: enable
    virtual ~BillboardComponent(){}
    BillboardComponent& operator=( const BillboardComponent& other ) = delete;
    BillboardComponent& operator=( BillboardComponent&& source ) = delete; // TODO: enable

    bool operator==( const BillboardComponent& other ) const = delete;

    // OpenGL context has to exist, when called.
    static bool Initialize();

    void AlwaysFaceCamera( bool value );
    bool IsAlwaysFacingCamera() const;

    void EnableMonochrome( bool enabled );
    void EnableMonochrome( bool enabled, const math::float3& color );

    void SetWidth( float width );
    void SetHeight( float height );
    void SetSize( float width, float height );
    void SetTexture( const std::string& name );
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
    static gl::Buffer<unsigned int> stIndexBuffer;  // @TODO: deprecated, delete later
    static const pt::Name stNameM;

    bool mUseColor = false;
    math::float3 mMonoColor = math::float3::white;
    std::string         mTextureName;
    gl::Texture2dPtr    mTexture;
    float mWidth    = 2.0f; // [-1, 1]
    float mHeight   = 2.0f; // [-1, 1]
    gl::Buffer<gl::Vertex>   mVertexBuffer;
    gl::Buffer<unsigned int> mIndexBuffer;
    gl::Buffer<math::vec3>   mNormalBuffer;      // used for displaying the normal vectors as lines (mVertexBuffer contains normal data for other usages)

    bool mAlwaysFaceCamera = false;
};

} // end of namespace 'engine'
