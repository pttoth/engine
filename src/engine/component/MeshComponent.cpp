#include "engine/component/MeshComponent.h"

#include "engine/service/AssetControl.h"
#include "engine/service/DrawingControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Services.h"
#include "engine/gl/Buffer.hpp"
//#include "engine/gl/ShaderProgram.h"
#include "engine/StandardShaderProgram.h"

#include "engine/Utility.h"

#include <vector>

using namespace engine;


MeshComponent::
MeshComponent( const std::string& name ):
    RealComponent( name )
{}


MeshComponent::
~MeshComponent()
{}


void MeshComponent::
SetMesh( const std::string& mesh_name )
{
    // skip re-creation of render context if it didn't have one, or the new mesh name is empty
    bool skip_reinit = (not IsRenderContextInitialized()) || (0 == mesh_name.length());

    DestroyContext();

    mMeshName = mesh_name;
    mMesh = nullptr;

    if( skip_reinit ){
        return;
    }

    auto ac = Services::GetAssetControl();
    gl::MeshPtr mesh = ac->GetMesh( mesh_name );
    if( nullptr == mesh ){
        PT_LOG_ERR( "Failed to retrieve mesh '" << mesh_name << "'" );
    }

    // mesh may be shared, so we do not destroy render context
    mMesh = mesh;
    CreateContext();
}


void MeshComponent::
SetUVScale( float scale )
{
    mFixUVScale = scale;
}

void MeshComponent::
EnableFixUVScale( bool enabled )
{
    mFixUVScaleEnabled = enabled;
}


void MeshComponent::
OnSpawned()
{}


void MeshComponent::
OnDespawned()
{}


void MeshComponent::
OnTick( float t, float dt )
{}


void MeshComponent::
OnDraw( float t, float dt )
{
    if( !IsRenderContextInitialized() ){
        return;
    }

    auto dc = Services::GetDrawingControl();
    auto cam = dc->GetCurrentCamera();
    auto shaderProgram = dc->GetDefaultShaderProgram();

    shaderProgram->Use();
    shaderProgram->SetUniformModelMatrix( this->GetWorldTransform() );
    shaderProgram->SetUniformModelViewProjectionMatrix( CalcMVP( *this, *cam.get() ) );


    //TODO: 'mMesh' nullcheck needed?
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mMesh->GetVertexBuffer() );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, mMesh->GetIndexBuffer() );

    gl::EnableVertexAttribArray( 0 );
    gl::EnableVertexAttribArray( 1 );
    gl::EnableVertexAttribArray( 2 );

    gl::Vertex::SetPositionAttributePointer( 0 );
    gl::Vertex::SetTexelAttributePointer( 1 );
    gl::Vertex::SetNormalAttributePointer( 2 );

    //draw each mesh piece
    auto& indexcounts = mMesh->GetPieceIndexCounts();
    size_t startindex = 0;

    auto ac = Services::GetAssetControl();

    const auto& materials = mMesh->GetMaterials();
    for( size_t i=0; i<materials.size(); ++i ){
        gl::MaterialPtr m = materials[i];
        if( nullptr == m ){
            m = ac->GetFallbackMaterial();
            PT_LOG_LIMITED_ERR( 1000, "Mesh '" << mMesh->GetName() << "' contains 'nullptr' as material!" );
        }
        if( nullptr == m){
            PT_LOG_LIMITED_ERR( 50, "Acquired 'nullptr' as fallback material from Asset Control!" );
            return;
        }
        m->Bind();

        gl::DrawElements( gl::DrawMode::TRIANGLES,
                          indexcounts[i],
                          GL_UNSIGNED_INT,
                          (void*) (startindex * sizeof(int)) );

        startindex += indexcounts[i];
    }

    gl::DisableVertexAttribArray( 2 );
    gl::DisableVertexAttribArray( 1 );
    gl::DisableVertexAttribArray( 0 );

    PT_GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    PT_GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
}


bool MeshComponent::
OnCreateContext()
{
    auto ac = Services::GetAssetControl();
    assert( nullptr != ac );

    if( 0 == mMeshName.length() ){
        return false;
    }

    mMesh = ac->GetMesh( mMeshName );
    if( nullptr == mMesh ){
        PT_LOG_ERR( "Could not create rendercontext for mesh '" << mMeshName << "'" );
        return false;
    }
    mMesh->LoadToGPU();

    return true;
}


bool MeshComponent::
OnDestroyContext()
{
    auto ac = Services::GetAssetControl();
    ac->SafeReleaseMesh( mMesh->GetName().GetStdString() );
    return true;
}

