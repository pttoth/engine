#pragma once

#include "engine/experimental/RealComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class SphereComponent;
using SphereComponentPtr  = std::shared_ptr< SphereComponent >;
using SphereComponentPtrW = std::weak_ptr< SphereComponent >;

class SphereComponent: public RealComponent
{
public:
    SphereComponent( const std::string& name );
    SphereComponent( const SphereComponent& other );
    SphereComponent( SphereComponent&& source );

    virtual ~SphereComponent();

    SphereComponent& operator=( const SphereComponent& other );
    SphereComponent& operator=( SphereComponent&& source );

    bool operator==( const SphereComponent& other ) const;

    void Draw( float t, float dt ) override;

    void SetRadius( float r );
    float GetRadius() const;

protected:
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    float mRadius = 1.0f;

};

} //end of namespace experimental
} //end of namespace engine
