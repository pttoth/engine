#include "engine/component/MeshComponent.h"

#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/Services.h"


#include <vector>

using namespace engine;


MeshComponent::
MeshComponent( const std::string& name ):
    RealComponent( name )
{}


MeshComponent::
MeshComponent( MeshComponent&& source ):
    RealComponent( std::move( source ) )
{
    PT_UNIMPLEMENTED_FUNCTION
}


MeshComponent::
~MeshComponent()
{}


MeshComponent &MeshComponent::
operator=( MeshComponent&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
    return *this;
}


const std::string& MeshComponent::
GetMeshPath() const
{
    return mMeshName;
}


void MeshComponent::
SetMeshPath( const std::string& str )
{
    if( !IsRenderContextInitialized() ){
        mMeshName = str;
        return;
    }

    // TODO: if there is already a rendercontext, it needs to be updated here
    PT_UNIMPLEMENTED_FUNCTION
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
    const auto& materials = mMesh->GetMaterials();
    for( size_t i=0; i<materials.size(); ++i ){
        materials[i]->Bind(); // bind textures

        gl::DrawElements( gl::DrawMode::TRIANGLES,
                          //indexcounts[i] * 3,
                          indexcounts[i],
                          GL_UNSIGNED_INT,
                          (void*) (startindex * sizeof(int)) );
                          //(void*) (startindex * 3) );

        startindex += indexcounts[i];
    }

    gl::DisableVertexAttribArray( 2 );
    gl::DisableVertexAttribArray( 1 );
    gl::DisableVertexAttribArray( 0 );

    GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
}


bool MeshComponent::
OnCreateContext()
{
    auto ec = Services::GetEngineControl();
    auto ac = Services::GetAssetControl();
    assert( nullptr != ec );
    assert( nullptr != ac );

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
    mMesh->FreeVRAM();
    return true;
}

