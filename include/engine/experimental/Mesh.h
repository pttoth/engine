#pragma once

#include "pt/math.h"

#include "engine/experimental/Material.h"

#include <memory>
#include <vector>



namespace engine{
namespace experimental{


class Mesh;
using MeshPtr  = std::shared_ptr< Mesh >;
using MeshPtrW = std::weak_ptr< Mesh >;

class Mesh
{
public:
    Mesh();
    Mesh( const Mesh& other ) = default;
    Mesh( Mesh&& source ) = default;

    virtual ~Mesh();

    Mesh& operator=( const Mesh& other ) = default;
    Mesh& operator=( Mesh&& source ) = default;

    bool operator==( const Mesh& other ) const;

    const std::vector< pt::math::float3 >& GetVertices() const;
    const std::vector< pt::math::int2 >&   GetEdges() const;
    const std::vector< pt::math::int3 >&   GetFaces() const;
    const std::vector< MaterialPtrW >&     GetMaterials() const;

protected:
private:
    std::vector< pt::math::float3 > mVertices;
    std::vector< pt::math::int2 >   mEdges;
    std::vector< pt::math::int3 >   mFaces;
    std::vector< MaterialPtrW >     mMaterials;
};

} //end of namespace experimental
} //end of namespace engine
