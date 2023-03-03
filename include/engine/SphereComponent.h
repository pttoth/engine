#pragma once

#include "engine/RealComponent.h"

#include <memory>

namespace engine{

class SphereComponent;
using SphereComponentPtr  = std::shared_ptr< SphereComponent >;
using SphereComponentPtrW = std::weak_ptr< SphereComponent >;

class SphereComponent: public RealComponent
{
public:
    SphereComponent( const std::string& name );
    SphereComponent( const SphereComponent& other ) = delete;
    SphereComponent( SphereComponent&& source ) = delete;

    virtual ~SphereComponent();

    SphereComponent& operator=( const SphereComponent& other ) = delete;
    SphereComponent& operator=( SphereComponent&& source ) = delete;

    bool operator==( const SphereComponent& other ) const = delete;

    void Draw( float t, float dt ) override;

    void SetRadius( float r );
    float GetRadius() const;

protected:
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:
    float mRadius = 1.0f;

};

} //end of namespace engine
