#include "engine/component/BillboardComponent.h"

#include "engine/gl/Def.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/actor/Camera.h"
#include "engine/service/DrawingControl.h"
#include "engine/Services.h"
#include "engine/Utility.h"
#include "pt/utility.hpp"

using namespace engine;
using namespace math;

bool BillboardComponent::stInitialized = false;
gl::Buffer<unsigned int> BillboardComponent::stIndexBuffer = gl::Buffer<unsigned int>();
const pt::Name BillboardComponent::stNameM = pt::Name( "M" );

gl::Buffer<unsigned int> InitIndices()
{
    PT_LOG_DEBUG( "Initializing BillboardComponent indexbuffer" );
    gl::Buffer<unsigned int> indices = { 0, 2, 1, 2, 3, 1 };
    indices.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    return indices;
}


BillboardComponent::
BillboardComponent( const std::string& name ):
    RealComponent( name )
{}


bool BillboardComponent::
Initialize()
{
    if( !stInitialized ){
        stIndexBuffer = InitIndices();
        stNameM.Init();
        stInitialized = true;
    }
    return stInitialized;
}


void BillboardComponent::
EnableMonochrome( bool enabled )
{
    mUseColor = enabled;
}


void BillboardComponent::
EnableMonochrome( bool enabled, const math::float3& color )
{
    mMonoColor = color;
    EnableMonochrome( enabled );
}


void BillboardComponent::
OnDraw( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetMainCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();

    //TODO: remove these, these shouldn't be set per-instance
    gl::Uniform<int> uniColorMode = shaderProgram->GetUniform<int>( "ColorMode" );
    gl::Uniform<vec3> uniColor = shaderProgram->GetUniform<vec3>( "Color" );
    gl::Uniform<float> uniT = shaderProgram->GetUniform<float>( "t" );

    shaderProgram->Use();
    shaderProgram->SetUniformModelMatrix( this->GetWorldTransform() );
    shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );

    if( mUseColor ){
        shaderProgram->SetUniform<int>( uniColorMode, 1 ); // monochrome
        //shaderProgram->SetUniform<int>( uniColorMode, 2 ); // pulsing
        shaderProgram->SetUniform<vec3>( uniColor, mMonoColor );
    }else{
        if( mTexture ){
            auto texunit = dc->GetTextureUnit( mTexture );
            gl::ActiveTexture( texunit );
            mTexture->Bind();
        }
    }

    gl::EnableVertexAttribArray( 0 );
    gl::EnableVertexAttribArray( 1 );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, stIndexBuffer );
    gl::Vertex::SetPositionAttributePointer( 0 );
    gl::Vertex::SetTexelAttributePointer( 1 );

    gl::DrawElements( gl::DrawMode::TRIANGLES, 2*3, GL_UNSIGNED_INT, 0 ); // draw 2 triangles with 3-3 vertices

    gl::DisableVertexAttribArray( 1 );
    gl::DisableVertexAttribArray( 0 );
    GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
    GL_UnbindTexture( GL_TEXTURE_2D );
    if( mUseColor ){
        shaderProgram->SetUniform<int>( uniColorMode, 0 );
    }
}


void BillboardComponent::
SetWidth( float width )
{
    mWidth = width;
}


void BillboardComponent::
SetHeight( float height )
{
    mHeight = height;
}


void BillboardComponent::
SetSize( float width, float height )
{
    mWidth  = width;
    mHeight = height;
}


void BillboardComponent::
SetTexture( gl::Texture2dPtr texture )
{
    mTexture = texture;
}


void BillboardComponent::
OnSpawned()
{}


void BillboardComponent::
OnDespawned()
{}


void BillboardComponent::
OnTick( float t, float dt )
{}


bool BillboardComponent::
OnCreateContext()
{
    // No need for mutex here, Init is only done once.
    //  So no late-init here, rather an error msg to fix it.
    if( !stInitialized ){
        PT_LOG_ERR( "BillboardComponent is uninitialized!" );
        return false;
    }

    InitVertexData();
    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER,
                              gl::BufferHint::STATIC_DRAW );

    if( mTexture && (not mTexture->IsLoadedInVRAM()) && mTexture->IsLoadedInRAM() ){
        mTexture->LoadToVRAM();
    }

    return true;
}


bool BillboardComponent::
OnDestroyContext()
{
    mVertexBuffer.FreeVRAM();
    return true;
}


void BillboardComponent::
InitVertexData()
{
    if( 0 == mVertexBuffer.GetBytes() ){
        //set up billboard mesh (6 vertexes, 2 triangles)
        std::vector<gl::Vertex> data;
        data.reserve(4);
        float w = mWidth/2;
        float h = mHeight/2;
        gl::Vertex vx_topleft( math::float3( 0, -w, h ), // position
                               math::float2( 0, 1 ),     // texel
                               math::float3() );         // normal
        gl::Vertex vx_topright( math::float3( 0, w, h ), // position
                                math::float2( 1, 1 ),    // texel
                                math::float3() );        // normal
        gl::Vertex vx_bottomleft( math::float3( 0, -w, -h ), // position
                                  math::float2( 0, 0 ),      // texel
                                  math::float3() );          // normal
        gl::Vertex vx_bottomright( math::float3( 0, w, -h ), // position
                                   math::float2( 1, 0 ),     // texel
                                   math::float3() );         // normal
        data.push_back( vx_topleft );
        data.push_back( vx_topright );
        data.push_back( vx_bottomleft );
        data.push_back( vx_bottomright );

        mVertexBuffer = std::move( data );
    }
}
