#pragma once

#include "engine/component/RealComponent.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( SphereComponent )

class SphereComponent: public RealComponent
{
public:
    SphereComponent( const std::string& name );
    SphereComponent( const SphereComponent& other ) = delete;
    SphereComponent( SphereComponent&& source ) = delete; // TODO: enable

    virtual ~SphereComponent();

    SphereComponent& operator=( const SphereComponent& other ) = delete;
    SphereComponent& operator=( SphereComponent&& source ) = delete; // TODO: enable

    bool operator==( const SphereComponent& other ) const = delete;

    void Render_GL3_3( float t, float dt ) override;

    void SetRadius( float r );
    float GetRadius() const;

protected:
    bool OnCreateRenderContext_GL3_3() override;
    bool OnDestroyRenderContext_GL3_3() override;

private:
    float mRadius = 1.0f;

};

} //end of namespace engine
