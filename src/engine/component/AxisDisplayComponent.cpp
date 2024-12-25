#include "engine/component/AxisDisplayComponent.h"

#include "engine/service/DrawingControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Services.h"
#include "engine/StandardShaderProgram.h"


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
OnSpawned()
{}


void AxisDisplayComponent::
OnDespawned()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void AxisDisplayComponent::
OnTick( float t, float dt )
{}


gl::DrawStage AxisDisplayComponent::
GetPreferredDrawStage() const
{
    return gl::DrawStage::UI_WORLD;
}


void AxisDisplayComponent::
OnDraw( float t, float dt )
{
    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetCurrentCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();
    auto mUniAxisDrawMode = shaderProgram->GetUniform<int>( "AxisDrawMode" );

    shaderProgram->SetUniformModelMatrix( GetWorldTransform() );
    shaderProgram->SetUniform( mUniAxisDrawMode, 1 );
    gl::Disable( GL_DEPTH_TEST );
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mVertexBuffer );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, mIndexBuffer );
    gl::EnableVertexAttribArray( 0 );
    gl::Vertex::SetPositionAttributePointer( 0 );

    gl::DrawElements( gl::DrawMode::LINES, 6, GL_UNSIGNED_INT, 0 );

    gl::DisableVertexAttribArray( 0 );
    PT_GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    PT_GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
    gl::Enable( GL_DEPTH_TEST );
    shaderProgram->SetUniform( mUniAxisDrawMode, 0 );
}


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
