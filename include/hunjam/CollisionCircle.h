#pragma once

#include "engine/component/MeshComponent.h"

#include "engine/gl/Mesh.h"

namespace hunjam{

PT_FORWARD_DECLARE_CLASS( CollisionCircle )

void ExtractCollisionCirclesFromMesh( const engine::gl::Mesh& mesh );


class CollisionCircle: public engine::MeshComponent
{
public:



    CollisionCircle( const std::string& name );

    CollisionCircle() = delete;
    CollisionCircle( const CollisionCircle& other ) = delete;
    CollisionCircle( CollisionCircle&& source ) = delete;
    virtual ~CollisionCircle();
    CollisionCircle& operator=( const CollisionCircle& other ) = delete;
    CollisionCircle& operator=( CollisionCircle&& source ) = delete;

    bool operator==( const CollisionCircle& other ) const = delete;

protected:

private:

};

} // end of namespace 'hunjam'
