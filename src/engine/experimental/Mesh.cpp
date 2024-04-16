#include "engine/experimental/Mesh.h"

#include "engine/service/AssetControl.h"
#include "engine/EngineControl.h"
#include "engine/Services.h"

#include "engine/gl/AssimpConfig.h"
#include <assert.h>

using namespace engine;
using namespace math;

Mesh::Piece::
Piece()
{}


Mesh::Piece::
Piece( const std::string& name ):
    mName( name )
{}


Mesh::Piece::
Piece( Piece&& source ):
    mName( std::move( source.mName ) ),
    mVertices( std::move( source.mVertices ) ),
    mEdges( std::move( source.mEdges ) ),
    mFaces( std::move( source.mFaces ) ),
    mMaterial( std::move( source.mMaterial ) )
{
    source.Clear();
}


Mesh::Piece& Mesh::Piece::
operator=( Piece&& source )
{
    if( this != &source ){
        mName = std::move( source.mName );
        mVertices = std::move( source.mVertices );
        mEdges = std::move( source.mEdges );
        mFaces = std::move( source.mFaces );
        mMaterial = std::move( source.mMaterial );
        source.Clear();
    }
    return *this;
}


void Mesh::Piece::
Clear()
{
    mName.clear();
    mVertices.clear();
    mEdges.clear();
    mFaces.clear();
    mMaterial = nullptr;
}

/*
Mesh::
Mesh()
{}
*/

Mesh::
Mesh( const std::string& name ):
    mName( name )
{}


Mesh::
Mesh( Mesh&& source ):
    mName( std::move( source.mName ) ),
    mPieces( std::move( source.mPieces ) )
{
    source.Clear();
}


Mesh& Mesh::
operator=( Mesh&& source )
{
    if( this != &source ){
        mName = std::move( source.mName );
        mPieces = std::move( source.mPieces );
        source.Clear();
    }
    return *this;
}


void Mesh::
Clear()
{
    //TODO: this shouldn't be cleared?
    //  if this is cleared, then ReadFile has to set the name, when reading
    //  ... but then AssetManager() would have to refresh location of Mesh
    //mName.clear();

    mPieces.clear();
    PT_WARN_UNIMPLEMENTED_FUNCTION
}


void Mesh::
FreeVRAM()
{
    PT_UNIMPLEMENTED_FUNCTION
}


const gl::Buffer<int>& Mesh::
GetIndexBuffer() const
{
    return mIndexBuffer;
}


const std::vector<gl::MaterialPtr>& Mesh::
GetMaterials() const
{
    return mCacheMaterials;
}


pt::Name Mesh::
GetName() const
{
    return mName;
}


const gl::Buffer<gl::Vertex>& Mesh::
GetVertexBuffer() const
{
    return mVertexBuffer;
}


const std::vector<int>& Mesh::
GetPieceIndexCounts() const
{
    return mPieceIndexCount;
}


size_t Mesh::
GetVRAMBytes() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


bool Mesh::
IsLoadedInVRAM() const
{
    PT_UNIMPLEMENTED_FUNCTION
}


void Mesh::
LoadToGPU()
{
    if( mVertexBuffer.IsClientSideSynced()
        && mIndexBuffer.IsClientSideSynced() )
    {
        return;
    }

    mVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
    mIndexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );

    auto ac = Services::GetAssetControl();

    for( auto& p : mPieces ){
        auto& m = p.mMaterial;
        if( !m->IsClientSideSynced() ){
            m->LoadToVRAM();
        }
    }
}


void Mesh::
Print()
{
    PT_LOG_DEBUG( "sziaszex?" );
}


void Mesh::
ReadFile( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Mesh::ReadFile(): tried to empty path as file! Skipping." );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace();
        #endif
        return;
    }

    PT_LOG_INFO( "Loading mesh: '" << name << "'" );

    Clear();

    auto ac = Services::GetAssetControl();
    auto ec = Services::GetEngineControl();
    assert( nullptr != ec );
    assert( nullptr != ac );

    //TODO: remove resolvemediafilepath from here!
    std::string assimpconfig_filepath = ec->ResolveMediaFilePath( ac->ResolveAssimpConfigFileName( name ) );
    AssimpConfig cfg;
    try{
        cfg.readF( assimpconfig_filepath );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Couldn't read assimp config file '" << assimpconfig_filepath << "'!" );
        return;
    }

    //TODO: could make a global adaptermap, that'll translate all Doom3 models?
    std::string adapter_filepath = ec->ResolveMediaFilePath( ac->ResolveMeshAdapterFileName( name ) );
    AdapterMap adapter = ReadAdapterMap( adapter_filepath );
    if( 0 == adapter.size() ){
        PT_LOG_DEBUG( "Couldn't read or non-existent adapter info '" << adapter_filepath << "'" );
    }

    MeshLoaderPtr ml = ac->GetMeshLoader();
    if( nullptr == ml ){
        PT_LOG_ERR( "Could not acquire mesh loader!" );
        return;
    }
    ml->FreeScene();

    //aiComponent
    int componentsToRemove = 0;
    //componentsToRemove |= aiComponent_NORMALS;
    //componentsToRemove |= aiComponent_TANGENTS_AND_BITANGENTS;
    //componentsToRemove |= aiComponent_COLORS;
    //componentsToRemove |= aiComponent_TEXCOORDS;
    //componentsToRemove |= aiComponent_BONEWEIGHTS;
    //componentsToRemove |= aiComponent_ANIMATIONS;
    //componentsToRemove |= aiComponent_TEXTURES;
    //componentsToRemove |= aiComponent_LIGHTS;
    //componentsToRemove |= aiComponent_CAMERAS;
    //componentsToRemove |= aiComponent_MESHES;
    //componentsToRemove |= aiComponent_MATERIALS;
    ml->AssimpSetPropertyInteger( AI_CONFIG_PP_RVC_FLAGS, componentsToRemove );
    std::string mesh_filename = ec->ResolveMediaFilePath( ac->ResolveMeshFileName( name ) );
    const aiScene* scene = ml->AssimpLoadMesh( mesh_filename, cfg.GetParameterMask() );
    if( nullptr == scene ){
        PT_LOG_ERR( "Could not load mesh '" << mesh_filename << "'" );
        return;
    }

    std::vector<int>        comp_idxcount;
    std::vector<gl::Vertex> vertices;
    std::vector<int>        indices;
    std::vector<gl::MaterialPtr> materials;
//TODO: pre allocate the vectors above
//...

    materials.reserve( scene->mNumMeshes );

    mPieces.reserve( scene->mNumMeshes );
    mPieceOffets.reserve( scene->mNumMeshes );
    for( size_t idx_piece=0; idx_piece < scene->mNumMeshes; ++idx_piece ){
        aiMesh* piece = scene->mMeshes[idx_piece];

        Mesh::Piece meshPiece( FixMeshName( piece->mName.C_Str() ) );
        meshPiece.mVertices.reserve( piece->mNumVertices );
        meshPiece.mFaces.reserve( piece->mNumFaces );

        //setup vertices
        ml->PrintScene( scene, "" );

        // submeshes all refer to vertices from 0,
        //  but all vertices are handled in one buffer,
        //  so indexes have to be offset by the existing vertex count for each submesh
        size_t idx_offset = vertices.size();

        for( size_t idx_vertex=0; idx_vertex<piece->mNumVertices; ++idx_vertex ){
            static const aiVector3D nullvector( 0, 0, 0 );
            const aiVector3D& p = ( piece->HasPositions() )         ? piece->mVertices[idx_vertex] : nullvector;
            const aiVector3D& t0 = ( piece->HasTextureCoords(0) )   ? piece->mTextureCoords[0][idx_vertex] : nullvector;
            //const aiVector3D& t1 = ( piece->HasTextureCoords(1) )   ? piece->mTextureCoords[1][idx_vertex] : nullvector;
            const aiVector3D& n = ( piece->HasNormals() )           ? piece->mNormals[idx_vertex] : nullvector;

            const gl::Vertex v( vec3( p.x, p.y, p.z ),
                                vec2( t0.x, t0.y ),
                                vec3( n.x, n.y, n.z ) );
            meshPiece.mVertices.push_back( v );
            vertices.push_back( v );
        }

        //set up faces
        size_t idxcount = 0;

        for( size_t idx_face=0; idx_face<piece->mNumFaces; ++idx_face ){
            aiFace& face = piece->mFaces[idx_face];
            if( 3 != face.mNumIndices ){
                PT_LOG_DEBUG( "Face vertex count differs from '3' in mesh file '" << mesh_filename << "'" );
                PT_UNIMPLEMENTED_FUNCTION
                //TODO: break up face into triangles
                //idxcount += ???;
            }else{
                meshPiece.mFaces.push_back( math::int3( face.mIndices[0], face.mIndices[1], face.mIndices[2] ) );
                indices.push_back( face.mIndices[0] + idx_offset );
                indices.push_back( face.mIndices[1] + idx_offset );
                indices.push_back( face.mIndices[2] + idx_offset );
                idxcount += 3;
            }
        }
        comp_idxcount.push_back( idxcount );

        //setup material
        meshPiece.mName = TranslateMaterialName( FixMeshName(piece->mName.C_Str() ), adapter );
        meshPiece.mMaterial = ac->GetMaterial( meshPiece.mName );

        mCacheMaterials.push_back( meshPiece.mMaterial );
        mPieces.push_back( std::move( meshPiece ) );
        PT_LOG_DEBUG( "Kakiszex" );
        std::cerr << "Kakiszex\n";
        pt::PrintStackTrace();
    }

    mVertexBuffer    = std::move( vertices );
    mIndexBuffer     = std::move( indices );
    mPieceIndexCount = std::move( comp_idxcount );

    //mName = name;  //TODO: remove | changed name messes up AssetManager's storage

    ml->FreeScene();
    PT_LOG_INFO( "Loaded mesh '" << GetName() << "'" );
}


Mesh::AdapterMap Mesh::
ReadAdapterMap( const std::string& path )
{
    if( 0 == path.length() ){
        PT_LOG_ERR( "Empty path while trying to read Mesh::AdapterMap" );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace();
        #endif
        return AdapterMap{};
    }

    std::ifstream ifs;
    ifs.open( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Could not open file '" << path << "'" );
        return AdapterMap{};
    }

    AdapterMap adapter;
    std::string res[2];
    std::string& left = res[0];
    std::string& right = res[1];
    for ( std::string currentline; std::getline(ifs, currentline); ){
        if( pt::SplitString( res, currentline, ";" ) ){
            left = pt::TrimWhitespaces( left );
            right = pt::TrimWhitespaces( right );
            adapter[left] = right;
        }
    }
    ifs.close();
    return adapter;
}


std::string Mesh::
TranslateMaterialName( const std::string& name, AdapterMap& adapter ) const
{
    if( 0 == adapter.count( name ) ){
        PT_LOG_WARN( "Could not find name '" << name << "' in adapter map." );
        return name;
    }
    const std::string& newname = adapter[name];
    PT_LOG_DEBUG( "Replaced name '" << name << "' to '"<< newname << "'." );
    return newname;
}


std::string Mesh::
FixMeshName( const std::string& name ) const
{
    static const char*  suffix = ".msh";
    static const size_t suffix_length = strlen( suffix );

    if( suffix == name.substr( name.length() - suffix_length, suffix_length ) ){
        std::string newname = name.substr( 0, name.length() - suffix_length );
        PT_LOG_DEBUG( "Fixed assimp-modified name: '" << name << "' -> '" << newname << "'" );
        return newname;
    }

    return name;
}
