#pragma once

#include "engine/component/RealComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightPointComponent )

class LightPointComponent: public RealComponent
{
public:
    LightPointComponent( const std::string& name );
    virtual ~LightPointComponent();

    LightPointComponent() = delete;
    LightPointComponent( const LightPointComponent& other ) = delete;
    LightPointComponent( LightPointComponent&& source ) = delete;
    LightPointComponent& operator=( const LightPointComponent& other ) = delete;
    LightPointComponent& operator=( LightPointComponent&& source ) = delete;
    bool operator==( const LightPointComponent& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnDraw( float t, float dt ) override;
    bool OnCreateContext() override;
    bool OnDestroyContext() override;

private:


};

} // end of namespace 'engine'
