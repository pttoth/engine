#pragma once

#include "engine/component/RealComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightComponentBase )


class LightComponentBase: public RealComponent
{
public:
    LightComponentBase( const std::string& name );
    virtual ~LightComponentBase();

    LightComponentBase() = delete;
    LightComponentBase( const LightComponentBase& other ) = delete;
    LightComponentBase( LightComponentBase&& source ) = delete;
    LightComponentBase& operator=( const LightComponentBase& other ) = delete;
    LightComponentBase& operator=( LightComponentBase&& source ) = delete;
    bool operator==( const LightComponentBase& other ) const = delete;


    virtual void LoadParametersToCurrentShader() = 0;

    virtual void EnableLight( bool val ) = 0;   // @TODO: use RealComponent::EnableDraw() as control logic
    virtual bool IsLightEnabled() const = 0;

    virtual void SetColor( math::vec3 color ) = 0;
    virtual void SetIntensity( float intensity ) = 0;

    virtual math::vec3  GetColor() const = 0;
    virtual float       GetIntensity() const = 0;

protected:

private:

};

} // end of namespace 'engine'
