#include "engine/component/MeshComponent.h"

#include "engine/service/AssetControl.h"
#include "engine/service/EngineControl.h"
#include "engine/service/Renderer.h"
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


gl::MeshPtr MeshComponent::
GetMesh()
{
    return mMesh;
}


void MeshComponent::
SetMesh( gl::MeshPtr mesh )
{
    // skip re-creation of render context if it didn't have one, or the new mesh is nullptr
    bool skip_reinit = (not IsRenderContextInitialized()) || (nullptr == mesh);

    DestroyRenderContext_GL3_3();

    mMeshName.clear();
    //mMesh = mesh;   //note: 'mMesh' is set by 'OnCreateContext()'

    if( skip_reinit ){
        return;
    }

    mMeshName = mesh->GetName();
    CreateRenderContext_GL3_3();
}


void MeshComponent::
SetMesh( const std::string& mesh_name )
{
    // skip re-creation of render context if it didn't have one, or the new mesh name is empty
    bool skip_reinit = (not IsRenderContextInitialized()) || (0 == mesh_name.length());

    DestroyRenderContext_GL3_3();

    mMeshName = mesh_name;
    mMesh = nullptr;

    if( skip_reinit ){
        return;
    }

    //mMesh = mesh;   //note: 'mMesh' is set by 'OnCreateContext()'
    CreateRenderContext_GL3_3();
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
OnRender_GL3_3( float t, float dt )
{
    if( !IsRenderContextInitialized() ){
        assert( IsRenderContextInitialized() );
        return;
    }

    auto dc = Services::GetRenderer();

    auto cam = dc->GetCurrentCamera();
    if( nullptr == cam ){
        PT_LOG_LIMITED_ERR( 100, "Problem with camera used for rendering! Skipping rendering." );
        return;
    }

    auto shaderProgram = dc->GetDefaultShaderProgram(); //@TODO: delete
                                                        //    use GetCurrentShaderProgram instead (need refactor first)
    if( (nullptr == shaderProgram)
        || (nullptr == shaderProgram->program)
        || (not shaderProgram->program->IsLinked()) )
    {
        PT_LOG_LIMITED_ERR( 100, "Tried to use invalid shaderprogram for rendering! Skipping rendering." );
        return;
    }

    // @TODO: There is no 'GetWorldRotationMtx()' function
    //  will only work with the root component for now...
    //shaderProgram->SetUniform( uniMrot, this->GetRotationMtx() );

    // find root component
    WorldComponent* root = this;
    WorldComponent* parent = root->GetParent();
    while( nullptr != parent ){
        root = parent;
        parent = root->GetParent();
    }


    shaderProgram->program->Use();
    shaderProgram->program->SetUniform( shaderProgram->uniM,    this->GetWorldTransform() );
    shaderProgram->program->SetUniform( shaderProgram->uniMrot, root->GetRotationMtx() );
    shaderProgram->program->SetUniform( shaderProgram->uniPVM,  CalcMVP( *this, *cam.get() ) );


    //TODO: 'mMesh' nullcheck needed?
    gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mMesh->GetVertexBuffer() );
    gl::BindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER, mMesh->GetIndexBuffer() );

    gl::EnableVertexAttribArray( 0 );
    gl::EnableVertexAttribArray( 1 );
    gl::EnableVertexAttribArray( 2 );

    gl::Vertex::SetPositionAttributePointer( 0 );
    gl::Vertex::SetTexelAttributePointer( 1 );
    gl::Vertex::SetNormalAttributePointer( 2 );


    // @TODO: mesh pieces should be queried at registration to renderer
    //          renderer should organize the pieces to material buckets
    //          then iterate on all buckets and all bucket contents

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

    // draw normal vectors, if needed in wireframe mode
    if( dc->GetWireframeMode() && dc->GetNormalVectorDisplay() ){
        size_t data_count = mMesh->GetNormalBuffer().GetDataRef().size();

        // @TODO: optimize (don't create uniform object on each draw)
        auto uniND = shaderProgram->program->GetUniform<int>( "NormalVectorDisplayMode" );
        shaderProgram->program->SetUniform( uniND, 1 );


        // @TODO: optimize: use VAO-s!!!!!!!!!!!!!!!!!
        gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mMesh->GetNormalBuffer() );
        gl::EnableVertexAttribArray( 0 );
        gl::VertexAttribPointer( 0, 3,
                                 GL_FLOAT, gl::SKIP_TRANSPOSE,
                                 sizeof(math::vec3), 0 );

        gl::DrawArrays( gl::DrawMode::LINES,
                          0,
                          data_count *3 );

        shaderProgram->program->SetUniform( uniND, 0 );
        gl::DisableVertexAttribArray( 0 );
    }

    //@TODO: after implementing VAO usage, this will become needless
    PT_GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
    PT_GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );
}


bool MeshComponent::
OnCreateRenderContext_GL3_3()
{
    auto ac = Services::GetAssetControl();
    assert( nullptr != ac );

    if( 0 == mMeshName.length() ){
        PT_LOG_WARN( "Tried to create context for unnamed mesh!" );
        PT_PRINT_DEBUG_STACKTRACE();
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
OnDestroyRenderContext_GL3_3()
{
    auto ac = Services::GetAssetControl();
    ac->SafeReleaseMesh( mMesh->GetName() );
    return true;
}

