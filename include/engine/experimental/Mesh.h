#pragma once

#include "engine/experimental/Material.h"
#include "pt/macros.h"
#include "pt/math.h"
#include <memory>
#include <vector>

namespace engine{

PT_FORWARD_DECLARE_CLASS( Mesh )

class Mesh
{
public:
    Mesh();
    //Mesh( const std::string& name );

    Mesh( const Mesh& other );
    Mesh( Mesh&& source );
    virtual ~Mesh();
    Mesh& operator=( const Mesh& other );
    Mesh& operator=( Mesh&& source );

    bool operator==( const Mesh& other ) const;

    const std::vector< math::float3 >& GetVertices() const;
    const std::vector< math::int2 >&   GetEdges() const;
    const std::vector< math::int3 >&   GetFaces() const;
    const std::vector< MaterialWPtr >& GetMaterials() const;

protected:
private:
    std::vector< math::float3 > mVertices;
    std::vector< math::int2 >   mEdges;
    std::vector< math::int3 >   mFaces;
    std::vector< MaterialWPtr > mMaterials;
};

} // end of namespace 'engine'
