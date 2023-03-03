#pragma once



#include <memory>



class MeshComponent;
using MeshComponentPtr  = std::shared_ptr< MeshComponent >;
using MeshComponentPtrW = std::weak_ptr< MeshComponent >;

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


