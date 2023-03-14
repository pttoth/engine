#include "engine/Mesh.h"

#include <assert.h>

using namespace engine;

Mesh::
Mesh()
{}

/*
Mesh::
Mesh( const Mesh& other )
{}


Mesh::
Mesh( Mesh&& source )
{}
*/

Mesh::
~Mesh()
{}

/*
Mesh &Mesh::
operator=( const Mesh& other )
{}


Mesh &Mesh::
operator=( Mesh&& source )
{}
*/

bool Mesh::
operator==( const Mesh& other ) const
{
    assert( false );
}


const std::vector< pt::math::float3 >& Mesh::
GetVertices() const
{
    return mVertices;
}


const std::vector< pt::math::int2 >& Mesh::
GetEdges() const
{
    return mEdges;
}


const std::vector< pt::math::int3 >& Mesh::
GetFaces() const
{
    return mFaces;
}


const std::vector<MaterialPtrW>& Mesh::
GetMaterials() const
{
    return mMaterials;
}

