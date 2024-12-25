#include "engine/component/BillboardComponent.h"

#include "engine/gl/Def.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/actor/Camera.h"
#include "engine/service/AssetControl.h"
#include "engine/service/DrawingControl.h"
#include "engine/Services.h"
#include "engine/Utility.h"
#include "pt/utility.hpp"

using namespace engine;
using namespace math;

bool BillboardComponent::stInitialized = false;
gl::Buffer<unsigned int> BillboardComponent::stIndexBuffer = gl::Buffer<unsigned int>();    // @TODO: deprecated, delete
const pt::Name BillboardComponent::stNameM = pt::Name( "M" );

// @TODO: deprecated, delete this
gl::Buffer<unsigned int> InitIndices()
{
    PT_LOG_DEBUG( "Initializing BillboardComponent indexbuffer" );
    gl::Buffer<unsigned int> indices = { 0, 2, 1, 2, 3, 1 };
    //indices.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    return indices;
}


BillboardComponent::
BillboardComponent( const std::string& name ):
    RealComponent( name )
{}


bool BillboardComponent::
Initialize()
{
    // @TODO: rewrite this, don't use static buffer nor static names
    if( !stInitialized ){
        //stIndexBuffer = InitIndices();
        stNameM.Init();
        stInitialized = true;
    }
    return stInitialized;
}


void BillboardComponent::
AlwaysFaceCamera( bool value )
{
    mAlwaysFaceCamera = value;
}


bool BillboardComponent::
IsAlwaysFacingCamera() const
{
    return mAlwaysFaceCamera;
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
OnRender_GL3_3( float t, float dt )
{
    auto dc = Services::GetRenderer();
    auto cam = dc->GetCurrentCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();

    //TODO: remove these, these shouldn't be set per-instance
    gl::Uniform<int> uniColorMode = shaderProgram->GetUniform<int>( "ColorMode" );
    gl::Uniform<vec3> uniColor = shaderProgram->GetUniform<vec3>( "Color" );
    gl::Uniform<float> uniT = shaderProgram->GetUniform<float>( "t" );

    shaderProgram->Use();
    if( mAlwaysFaceCamera ){
        mat4 rotmtx = this->GetLookAtRotation( dc->GetCurrentCamera()->GetWorldPosition() );
        mat4 parent_tf = this->GetParent()->GetWorldTransform();
        mat4 M = parent_tf * BuildTransformMtx( this->GetPosition(), rotmtx, this->GetScale() );
        shaderProgram->SetUniformModelMatrix( M );
        shaderProgram->SetUniformModelViewProjectionMatrix( cam->GetProjMtx() * cam->GetViewMtx() * M );
    }else{
        shaderProgram->SetUniformModelMatrix( this->GetWorldTransform() );
        shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );
    }

    if( mUseColor ){
        shaderProgram->SetUniform<int>( uniColorMode, 1 ); // monochrome
        //shaderProgram->SetUniform<int>( uniColorMode, 2 ); // pulsing
        shaderProgram->SetUniform<vec3>( uniColor, mMonoColor );
    }else{
        if( mTexture ){
            mTexture->BindToTextureUnit( dc->GetMainTextureUnit() );
        }
    }

    gl::EnableVertexAttribArray( 0 );
    gl::EnableVertexAttribArray( 1 );
    gl::EnableVertexAttribArray( 2 );

    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, mIndexBuffer );
    //gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, stIndexBuffer );  // @TODO: deprecated, delete
    gl::Vertex::SetPositionAttributePointer( 0 );
    gl::Vertex::SetTexelAttributePointer( 1 );
    gl::Vertex::SetNormalAttributePointer( 2 );

    // @TODO: draw GL_TRIANGLE_STRIP instead and drop index buffer
    gl::DrawElements( gl::DrawMode::TRIANGLES, 2*3, GL_UNSIGNED_INT, 0 ); // draw 2 triangles with 3-3 vertices

    gl::DisableVertexAttribArray( 2 );
    gl::DisableVertexAttribArray( 1 );
    gl::DisableVertexAttribArray( 0 );

    // draw normal vectors, if needed in wireframe mode
    if( dc->GetWireframeMode() && dc->GetNormalVectorDisplay() ){
        size_t data_count = mNormalBuffer.GetDataRef().size();

        // @TODO: optimize (don't create buffer object on each draw)
        auto uniND = shaderProgram->GetUniform<int>( "NormalVectorDisplayMode" );
        shaderProgram->SetUniform( uniND, 1 );

        gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mNormalBuffer );
        gl::EnableVertexAttribArray( 0 );
        gl::VertexAttribPointer( 0, 3,
                                 GL_FLOAT, gl::SKIP_TRANSPOSE,
                                 sizeof(math::vec3), 0 );

        gl::DrawArrays( gl::DrawMode::LINES,
                          0,
                          data_count *3 );

        shaderProgram->SetUniform( uniND, 0 );
        gl::DisableVertexAttribArray( 0 );
    }


    PT_GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    PT_GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
    PT_GL_UnbindTexture( GL_TEXTURE_2D );
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
SetTexture( const std::string& name )
{
    // skip re-creation of render context if it didn't have one, or the new mesh name is empty
    bool skip_reinit = (not IsRenderContextInitialized()) || (0 == name.length());

    DestroyRenderContext_GL3_3();

    mTextureName = "";
    mTexture = nullptr;

    if( skip_reinit ){
        return;
    }

    auto ac = Services::GetAssetControl();
    gl::Texture2dPtr tex = ac->GetTexture( name );
    if( nullptr == tex ){
        PT_LOG_ERR( "Failed to retrieve texture '" << name << "'" );
    }

    mTextureName = name;
    mTexture = tex;
    CreateRenderContext_GL3_3();
}


void BillboardComponent::
SetTexture( gl::Texture2dPtr texture )
{
    if( nullptr == texture ){
        mTexture = nullptr;
        mTextureName = "";
        return;
    }

    mTexture = texture;
    mTextureName = texture->GetName();
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
OnCreateRenderContext_GL3_3()
{
    // No need for mutex here, Init is only done once.
    //  So no late-init here, rather an error msg to fix it.
    if( !stInitialized ){
        PT_LOG_ERR( "BillboardComponent is uninitialized!" );
        return false;
    }

    InitVertexData();
    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER,   gl::BufferHint::STATIC_DRAW );
    mIndexBuffer.LoadToVRAM(  gl::BufferTarget::ARRAY_BUFFER,   gl::BufferHint::STATIC_DRAW );
    mNormalBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER,   gl::BufferHint::STATIC_DRAW );

    if( mTexture && (not mTexture->HasDataInVRAM()) && mTexture->HasDataInRAM() ){
        mTexture->LoadToVRAM();
    }

    return true;
}


bool BillboardComponent::
OnDestroyRenderContext_GL3_3()
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
        std::vector<math::vec3> normaldata;
        data.reserve(4);
        float w = mWidth/2;
        float h = mHeight/2;
        gl::Vertex vx_topleft( math::float3( 0, -w, h ), // position
                               math::float2( 0, 1 ),     // texel
                               math::float3::xUnit );    // normal
        gl::Vertex vx_topright( math::float3( 0, w, h ), // position
                                math::float2( 1, 1 ),    // texel
                                math::float3::xUnit );   // normal
        gl::Vertex vx_bottomleft( math::float3( 0, -w, -h ), // position
                                  math::float2( 0, 0 ),      // texel
                                  math::float3::xUnit );     // normal
        gl::Vertex vx_bottomright( math::float3( 0, w, -h ), // position
                                   math::float2( 1, 0 ),     // texel
                                   math::float3::xUnit );    // normal
        data.push_back( vx_topleft );
        data.push_back( vx_topright );
        data.push_back( vx_bottomleft );
        data.push_back( vx_bottomright );

        float normalVecLength = 10;
        for( auto& v : data ){
            normaldata.push_back( v.pos );
            normaldata.push_back( v.pos + (v.normal * normalVecLength) );
        }

        mVertexBuffer = std::move( data );
        mNormalBuffer = std::move( normaldata );

        // initialize Index buffer
        mIndexBuffer = gl::Buffer<unsigned int>( { 0, 2, 1, 2, 3, 1 } );
    }
}
