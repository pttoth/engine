#pragma once

#include "engine/actor/Actor.h"

#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/LightConeComponent.h"
#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightCone )

class LightCone: public Actor
{
public:
    LightCone( const std::string& name );
    virtual ~LightCone();

    LightCone( const LightCone& other ) = delete;
    LightCone( LightCone&& source ) = delete;
    LightCone& operator=( const LightCone& other ) = delete;
    LightCone& operator=( LightCone&& source ) = delete;

    bool operator==( const LightCone& other ) const = delete;

    void ShowMesh( bool val );
    void SetAngle( float val );

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    //bool    mShowMesh   = false;
    bool    mShowMesh   = true;

    engine::LightConeComponentPtr   mLightCone;
    engine::MeshComponentPtr        mMesh;
    engine::AxisDisplayComponentPtr mAxis;

};

} // end of namespace 'engine'
