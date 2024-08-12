#pragma once

#include "engine/actor/Actor.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightPoint )

class LightPoint: public Actor
{
public:
    LightPoint( const std::string& name );
    virtual ~LightPoint();

    LightPoint() = delete;    
    LightPoint( const LightPoint& other ) = delete;
    LightPoint( LightPoint&& source ) = delete;
    LightPoint& operator=( const LightPoint& other ) = delete;
    LightPoint& operator=( LightPoint&& source ) = delete;
    bool operator==( const LightPoint& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:

};

} // end of namespace 'engine'
