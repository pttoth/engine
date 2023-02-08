#pragma once



#include <memory>



class MeshComponent;
using MeshComponentPtr  = std::shared_ptr< MeshComponent >;
using MeshComponentPtrW = std::weak_ptr< MeshComponent >;

class MeshComponent
{
public:
    MeshComponent();
    MeshComponent( const MeshComponent& other );
    MeshComponent( MeshComponent&& source );

    virtual ~MeshComponent();

    MeshComponent& operator=( const MeshComponent& other );
    MeshComponent& operator=( MeshComponent&& source );

    bool operator==( const MeshComponent& other ) const;
protected:
private:
};


