#pragma once

#include "engine/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( ILight )

class ILight
{
public:
    virtual ~ILight(){}

    virtual void UpdateLightParametersInCurrentShader() = 0;    // @TODO: deprecated

    virtual void Enable( bool val ) = 0;

    virtual void SetColor( math::vec3 color ) = 0;
    virtual void SetIntensity( float intensity ) = 0;
    virtual void SetRadius( float radius ) = 0;

    virtual math::vec3  GetColor() const = 0;
    virtual float       GetIntensity() const = 0;
    virtual float       GetRadius() const = 0;

protected:

private:

};

} // end of namespace 'engine'
