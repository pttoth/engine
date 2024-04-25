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


Mesh::
Mesh( Mesh&& source ):
    mIsLoadedInVRAM( source.mIsLoadedInVRAM ),
    mName(      std::move( source.mName ) ),
    mPieces(    std::move( source.mPieces ) ),
    mVertexBuffer(  std::move( source.mVertexBuffer ) ),
    mIndexBuffer(   std::move( source.mIndexBuffer ) ),
    mPieceIndexCount(   std::move( source.mPieceIndexCount ) ),
    mMaterials(         std::move( source.mMaterials ) )
{
    source.SetDefaultMemberValues();
}


Mesh& Mesh::
operator=( Mesh&& source )
{
    if( this != &source ){
        mIsLoadedInVRAM     = source.mIsLoadedInVRAM;
        mName               = std::move( source.mName );
        mPieces             = std::move( source.mPieces );
        mVertexBuffer       = std::move( source.mVertexBuffer );
        mIndexBuffer        = std::move( source.mIndexBuffer );
        mPieceIndexCount    = std::move( source.mPieceIndexCount );
        mMaterials          = std::move( source.mMaterials );
        source.SetDefaultMemberValues();
    }
    return *this;
}


MeshPtr Mesh::
CreateFromAssimpScene( const std::string& name, const aiScene* scene, const AdapterMap* adapter )
{
    std::vector<size_t>          comp_idxcount;
    std::vector<Piece>           pieces;
    std::vector<gl::Vertex>      vertices;       // goes in mVertexBuffer
    std::vector<int>             indices;        // goes in mIndexBuffer
    std::vector<gl::MaterialPtr> materials;

    // calculate sizes
    size_t meshcount        = scene->mNumMeshes;
    size_t totalvertexcount = 0;
    size_t totalindexcount  = 0;

    for( size_t i=0; i<meshcount; ++i ){
        aiMesh* m = scene->mMeshes[i];
        totalvertexcount += m->mNumVertices;
        totalindexcount  += m->mNumFaces * 3; // assumes, that faces are only triangles!
    }

    comp_idxcount.reserve( meshcount );
    pieces.reserve( meshcount );
    vertices.reserve( totalvertexcount );
    indices.reserve( totalindexcount );
    materials.reserve( meshcount ); // this is the max needed size, might not be fully filled

    auto ac = Services::GetAssetControl();
    if( nullptr == ac ){
        PT_LOG_ERR( "No AssetManager available!" );
        assert( nullptr != ac );
        return nullptr;
    }

    auto ec = Services::GetEngineControl();
    if( nullptr == ec ){
        PT_LOG_ERR( "No EngineControl available!" );
        assert( nullptr != ec );
        return nullptr;
    }

    for( size_t idx_piece=0; idx_piece<meshcount; ++idx_piece ){
        const aiMesh* piece = scene->mMeshes[idx_piece];

        Mesh::Piece meshPiece( FixMeshName( piece->mName.C_Str() ) ); // removes '.msh' suffix
        meshPiece.mVertices.reserve( piece->mNumVertices );
        meshPiece.mFaces.reserve( piece->mNumFaces );

        // assimp submeshes all refer to vertices from 0 upwards,
        //  but here, all vertices are handled in one buffer,
        //  so indexes have to be offset by the existing vertex count for each submesh
        size_t idx_offset = vertices.size();
        size_t piece_vtx_count = piece->mNumVertices;

        for( size_t idx_vertex=0; idx_vertex < piece_vtx_count; ++idx_vertex ){
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
                PT_LOG_ERR( "Not all faces are triangles in assimp mesh piece'" << piece->mName.C_Str() << "'" );
                PT_UNIMPLEMENTED_FUNCTION
                //TODO: break up face into triangles
                //          for now, use the Assimp flag for this

            }else{
                meshPiece.mFaces.push_back( math::int3( face.mIndices[0], face.mIndices[1], face.mIndices[2] ) );
                indices.push_back( face.mIndices[0] + idx_offset );
                indices.push_back( face.mIndices[1] + idx_offset );
                indices.push_back( face.mIndices[2] + idx_offset );
                idxcount += 3;
            }
        }
        comp_idxcount.push_back( idxcount );

        // setup material | apply adapter, if available
        if( nullptr == adapter ){
            meshPiece.mName = FixMeshName( piece->mName.C_Str() );
        }else{
            meshPiece.mName = TranslateMaterialName( FixMeshName( piece->mName.C_Str() ), *adapter );
        }
        meshPiece.mMaterial = ac->GetMaterial( meshPiece.mName );

        materials.push_back( meshPiece.mMaterial );
        pieces.push_back( std::move( meshPiece ) );
    }

    MeshPtr instance            = MeshPtr( new Mesh( name ) ); // workaround, because 'NewPtr' here wouldn't access the protected constructor
    instance->mPieceIndexCount  = std::move( comp_idxcount );
    instance->mVertexBuffer     = std::move( vertices );
    instance->mIndexBuffer      = std::move( indices );
    instance->mPieces           = std::move( pieces );
    instance->mMaterials        = std::move( materials );

    return instance;
}


MeshPtr Mesh::
CreateFromFileAssimp( const std::string& name )
{
    if( 0 == name.length() ){
        PT_LOG_ERR( "Mesh::ReadFile(): tried to empty path as file! Skipping." );
        #ifdef PT_DEBUG_ENABLED
            pt::PrintStackTrace();
        #endif
        return nullptr;
    }

    PT_LOG_INFO( "Loading mesh: '" << name << "'" );

    auto ac = Services::GetAssetControl();
    auto ec = Services::GetEngineControl();
    assert( nullptr != ac );
    assert( nullptr != ec );

    //TODO: remove resolvemediafilepath from here!
    std::string assimpconfig_filepath = ec->ResolveMediaFilePath( ac->ResolveAssimpConfigFileName( name ) );
    AssimpConfig cfg;
    try{
        cfg.ReadFile( assimpconfig_filepath );
    }catch( const std::invalid_argument& e ){
        PT_LOG_ERR( "Couldn't read assimp config file '" << assimpconfig_filepath << "'!" );
        cfg.SetDefaults();
    }

    //TODO: could make a global adaptermap, that'll translate all Doom3 models?
    std::string adapter_filepath = ec->ResolveMediaFilePath( ac->ResolveMeshAdapterFileName( name ) );
    AdapterMap adapter = Mesh::ReadAdapterMap( adapter_filepath );
    if( 0 == adapter.size() ){
        PT_LOG_DEBUG( "Couldn't read or non-existent adapter info '" << adapter_filepath << "'" );
    }

    MeshLoaderPtr ml = ac->GetMeshLoader();
    if( nullptr == ml ){
        PT_LOG_ERR( "Could not acquire mesh loader!" );
        return nullptr;
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
        return nullptr;
    }

    MeshPtr instance = Mesh::CreateFromAssimpScene( name, scene, &adapter );

    ml->FreeScene();
    PT_LOG_INFO( "Loaded mesh '" << instance->GetName() << "'" );
    return instance;
}


void Mesh::
FreeClientsideData()
{
    mPieces = std::vector<Piece>();
}


void Mesh::
FreeVRAM()
{
    PT_UNIMPLEMENTED_FUNCTION
    mIsLoadedInVRAM = false;
}


const gl::Buffer<int>& Mesh::
GetIndexBuffer() const
{
    return mIndexBuffer;
}


const std::vector<gl::MaterialPtr>& Mesh::
GetMaterials() const
{
    return mMaterials;
}


pt::Name Mesh::
GetName() const
{
    return mName;
}


const std::vector<size_t>& Mesh::
GetPieceIndexCounts() const
{
    return mPieceIndexCount;
}


const gl::Buffer<gl::Vertex>& Mesh::
GetVertexBuffer() const
{
    return mVertexBuffer;
}


size_t Mesh::
GetVRAMBytes() const
{
    return mVertexBuffer.GetVRAMBytes() + mIndexBuffer.GetVRAMBytes();
}


bool Mesh::
IsLoadedInVRAM() const
{
    return mIsLoadedInVRAM;
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
    mIsLoadedInVRAM = true;
}


void Mesh::
Print()
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
}


// --------------------------------------------------
//  protected functions
// --------------------------------------------------


Mesh::
Mesh()
{}


Mesh::
Mesh( const std::string& name ):
    mName( name )
{}


Mesh::AdapterMap Mesh::
CreateAdapterMap( const aiScene* scene )
{
    PT_UNIMPLEMENTED_FUNCTION
}


std::string Mesh::
FixMeshName( const std::string& name )
{
    static const char*  suffix = ".msh";
    static const size_t suffix_length = strlen( suffix );

    // if tail of string is ".msh"
    if( suffix == name.substr( name.length() - suffix_length, suffix_length ) ){
        std::string newname = name.substr( 0, name.length() - suffix_length );
        PT_LOG_DEBUG( "Fixed assimp-modified name: '" << name << "' -> '" << newname << "'" );
        return newname;
    }else{
        PT_LOG_DEBUG( "Skipped fixing mesh name '" << name << "' as suffix '" << suffix << "'was not found" );
    }

    return name;
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

    std::ifstream ifs( path );
    if( !ifs.is_open() ){
        PT_LOG_ERR( "Could not open file '" << path << "'" );
        return AdapterMap{};
    }

    AdapterMap adapter;
    std::string res[2];
    std::string& left = res[0];
    std::string& right = res[1];
    for ( std::string currentline; std::getline( ifs, currentline ); /* empty */ ){
        if( pt::SplitString( res, currentline, ";" ) ){
            left  = pt::TrimWhitespaces( left );
            right = pt::TrimWhitespaces( right );
            adapter[left] = right;
        }
    }
    ifs.close();
    return adapter;
}


std::string Mesh::
TranslateMaterialName( const std::string& name, const AdapterMap& adapter )
{
    if( 0 == adapter.count( name ) ){
        PT_LOG_WARN( "Could not find name '" << name << "' in adapter map." );
        return name;
    }
    const std::string& newname = adapter.at(name);
    PT_LOG_DEBUG( "Replaced name '" << name << "' to '"<< newname << "'." );
    return newname;
}
