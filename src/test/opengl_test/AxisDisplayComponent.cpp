#include "test/opengl_test/AxisDisplayComponent.h"

#include "engine/DrawingManager.h"
#include "engine/EngineControl.h"
#include "engine/DefaultShaderProgram.h"
#include "engine/Services.h"

using namespace engine;
using namespace math;


AxisDisplayComponent::
~AxisDisplayComponent()
{}


AxisDisplayComponent::
AxisDisplayComponent( const std::string& name ):
    RealComponent( name )
{}


void AxisDisplayComponent::
OnDraw( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetMainCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();
    auto mUniDrawingAxes = shaderProgram->GetUniform<int>( "DrawingAxes" );
    mat4 origoTransform = cam->GetProjMtx() * cam->GetViewMtx() * GetWorldTransform();

    shaderProgram->SetUniformModelViewProjectionMatrix( origoTransform );
    shaderProgram->SetUniform( mUniDrawingAxes, 1 );
    gl::Disable( GL_DEPTH_TEST );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, mIndexBuffer );
    gl::EnableVertexAttribArray( 0 );
    gl::Vertex::SetPositionAttributePointer( 0 );

    gl::DrawElements( gl::DrawMode::LINES, 6, GL_UNSIGNED_INT, 0 );

    gl::DisableVertexAttribArray( 0 );
    GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
    gl::Enable( GL_DEPTH_TEST );
    shaderProgram->SetUniform( mUniDrawingAxes, 0 );
}


void AxisDisplayComponent::
OnTick( float t, float dt )
{}


bool AxisDisplayComponent::
OnCreateContext()
{
    {
        std::vector<unsigned int> ivec = { 0, 1, 0, 2, 0, 3 };
        std::vector<gl::Vertex> vertices;
        vertices.reserve(4);
        vertices.push_back( vec3::zero );
        vertices.push_back( vec3::xUnit );
        vertices.push_back( vec3::yUnit );
        vertices.push_back( vec3::zUnit );
        mVertexBuffer = std::move( vertices );
        mIndexBuffer = std::move(ivec);
    }

    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    mIndexBuffer.LoadToVRAM( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );

    return true;
}


bool AxisDisplayComponent::
OnDestroyContext()
{
    mVertexBuffer.FreeVRAM();
    mVertexBuffer = gl::Buffer<gl::Vertex>();
    mIndexBuffer.FreeVRAM();
    mIndexBuffer = gl::Buffer<unsigned int>();
    return true;
}


void AxisDisplayComponent::
OnSpawned()
{
    //add this component to the drawing manager
    auto dc = Services::GetDrawingControl();
    dc->AddDrawable( this );
}


void AxisDisplayComponent::
OnDespawned()
{
    PT_UNIMPLEMENTED_FUNCTION
}


gl::DrawStage AxisDisplayComponent::
GetPreferredDrawStage() const
{
    return gl::DrawStage::UI_WORLD;
}

