#pragma once

#include "engine/actor/Actor.h"
#include "engine/interface/ILight.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Light )

class Light: public Actor,
             public ILight
{
public:
    Light( const std::string& name );

    Light( const Light& other ) = delete;
    Light( Light&& source ) = delete;
    virtual ~Light();
    Light& operator=( const Light& other ) = delete;
    Light& operator=( Light&& source ) = delete;

    bool operator==( const Light& other ) const = delete;

    // ILight interface
    void        SetLightColor( const math::vec4& lightcolor ) override;
    math::vec4  GetLightColor() const override;
    void        LoadToVRAM() override;


protected:
    // Actor interface
    void    OnTick( float t, float dt ) override;
    void    OnSpawned() override;
    void    OnDespawned() override;
    bool    OnCreateRenderContext() override;
    void    OnDestroyRenderContext() override;


private:


};

} // end of namespace 'engine'
