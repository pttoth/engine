#pragma once

#include "engine/actor/Light.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

//TODO: fix .cpp code style

PT_FORWARD_DECLARE_CLASS( LightAmbient )

class LightAmbient: public Light
{
public:
    LightAmbient( const std::string& name );

    LightAmbient( const LightAmbient& other ) = delete;
    LightAmbient( LightAmbient&& source ) = delete;
    virtual ~LightAmbient();
    LightAmbient& operator=( const LightAmbient& other ) = delete;
    LightAmbient& operator=( LightAmbient&& source ) = delete;

    bool operator==( const LightAmbient& other ) const = delete;

    // ILight interface
    void        SetLightColor( const math::vec4& lightcolor );
    math::vec4  GetLightColor() const;
    void        LoadToVRAM();


protected:
    // Actor interface
    void    OnTick(float t, float dt);
    void    OnSpawned();
    void    OnDespawned();
    bool    OnCreateRenderContext();
    void    OnDestroyRenderContext();


private:


};

} // end of namespace 'engine'
