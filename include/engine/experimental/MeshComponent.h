#pragma once

#include "pt/macros.h"
#include <memory>

namespace engine{

PT_FORWARD_DECLARE_CLASS( MeshComponent )

class MeshComponent
{
public:
    MeshComponent();
    MeshComponent( const MeshComponent& other ) = delete;
    MeshComponent( MeshComponent&& source ) = delete;

    virtual ~MeshComponent();

    MeshComponent& operator=( const MeshComponent& other ) = delete;
    MeshComponent& operator=( MeshComponent&& source ) = delete;

    bool operator==( const MeshComponent& other ) const = delete;
protected:
private:
};

} // end of namespace 'engine'
