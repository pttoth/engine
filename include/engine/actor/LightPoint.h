#pragma once

#include "engine/actor/Actor.h"
#include "engine/interface/ILight.h"

//@TODO: revise
#include "engine/component/AxisDisplayComponent.h"
#include "engine/component/LightPointComponent.h"
#include "engine/component/MeshComponent.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( LightPoint )

class LightPoint: public Actor,
                  public ILight
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

    void UpdateLightParametersInCurrentShader() override;

    void Enable( bool val ) override;

    void SetColor( math::vec3 color ) override;
    void SetIntensity( float intensity ) override;
    void SetRadius( float radius ) override;

    void ShowMesh( bool val );

    math::vec3  GetColor() const override;
    float       GetIntensity() const override;
    float       GetRadius() const override;


protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:
    //bool    mShowMesh   = false;
    bool    mShowMesh   = true;

    engine::LightPointComponentPtr  mLightPointComponent;
    engine::MeshComponentPtr        mMesh;
    engine::AxisDisplayComponentPtr mAxis;



};

} // end of namespace 'engine'
