#include "engine/BillboardComponent.h"

#include "engine/gl/Def.h"
#include "pt/utility.hpp"
#include <array>

using namespace engine;

gl::Buffer<unsigned int> InitIndices()
{
    PT_LOG_DEBUG( "Initializing BillboardComponent indexbuffer" );
    gl::Buffer<unsigned int> indices = { 0, 1, 2, 1, 3, 2 };
    indices.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    return indices;
}


static const gl::Buffer<unsigned int> stIndexBuffer = InitIndices();


BillboardComponent::
BillboardComponent( const std::string& name ):
    RealComponent( name )
{}


void BillboardComponent::
OnDraw( float t, float dt )
{


    gl::EnableVertexAttribArray( 0 );
    gl::VertexAttribPointer( 0, 3,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_POSITION );
    gl::EnableVertexAttribArray( 1 );
    gl::VertexAttribPointer( 1, 3,
                             GL_FLOAT, gl::SKIP_NORMALIZE,
                             sizeof(gl::Vertex), PT_GL_VERTEX_OFFSET_TEXTURE );
    gl::DisableVertexAttribArray( 2 ); // no normal needed

    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, stIndexBuffer );
    gl::DrawElements( gl::DrawMode::TRIANGLES, 2*3, GL_UNSIGNED_INT, 0 ); // draw 2 triangles with 3 vertices
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, 0 );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, 0 );
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
OnSpawned()
{
    //add this component to the drawing manager

}


void BillboardComponent::
OnDespawned()
{
    //remove this component from the drawing manager
}


void BillboardComponent::
OnTick( float t, float dt )
{}


bool BillboardComponent::
OnCreateContext()
{
    InitVertexData();
    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER,
                              gl::BufferHint::STATIC_DRAW );
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
    if( 0 == mVertexBuffer.GetSize() ){
        //set up billboard mesh (6 vertexes, 2 triangles)
        std::vector<gl::Vertex> data;
        data.reserve(4);
        float w = mWidth/2;
        float h = mHeight/2;
        gl::Vertex vx_topleft( math::float3( -w, h, 0 ),   // position
                               math::float2( 0.0f, 1.0f ), // texel
                               math::float3() );           // normal
        gl::Vertex vx_topright( math::float3( w, h, 0 ),    // position
                                math::float2( 1.0f, 1.0f ), // texel
                                math::float3() );           // normal
        gl::Vertex vx_bottomleft( math::float3( -w, -h, 0 ),  // position
                                  math::float2( 0.0f, 0.0f ), // texel
                                  math::float3() );           // normal
        gl::Vertex vx_bottomright( math::float3( w, -h, 0 ),   // position
                                   math::float2( 1.0f, 0.0f ), // texel
                                   math::float3() );           // normal
        data.push_back( vx_topleft );
        data.push_back( vx_topright );
        data.push_back( vx_bottomleft );
        data.push_back( vx_bottomright );

        mVertexBuffer = std::move( data );
    }
}
