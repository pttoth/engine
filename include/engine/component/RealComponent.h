/** -----------------------------------------------------------------------------
  * FILE:    RealComponent.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Component with geometrical representation (for visuals or for physics)
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "WorldComponent.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include <memory>

// @TODO: add pure virtual 'LoadModelTransformDataToCurrentShader()' here
//      dependants should use it to update M, Mrot, etc. matrices, that aren't in uniform blocks

// @TODO: add 'ReceivesShadows( bool )'

namespace engine{

PT_FORWARD_DECLARE_CLASS( RealComponent )

class RealComponent: public WorldComponent
{
public:
    RealComponent( const std::string& name, bool drawEnabled = true );
    RealComponent( const RealComponent& other ) = delete;
    RealComponent( RealComponent&& source );

    virtual ~RealComponent();

    RealComponent& operator=( const RealComponent& other ) = delete;
    RealComponent& operator=( RealComponent&& source );

    bool operator==( const RealComponent& other ) const = delete;

    void SetPreferredDrawStage( gl::RenderStage draw_stage );
    virtual gl::RenderStage GetPreferredDrawStage() const;

    void Spawn() override;
    void Despawn() override;

    virtual void CreateRenderContext_GL3_3();
    virtual void CreateRenderContext_GL4_6();

    virtual void DestroyRenderContext_GL3_3();
    virtual void DestroyRenderContext_GL4_6();

    //@TODO: revise how this works with different renderers
    virtual void Render_GL3_3( float t, float dt );
    virtual void Render_GL4_6( float t, float dt );

    void EnableDraw( bool enabled );
    bool IsDrawEnabled() const;

    bool IsRenderContextInitialized() const;

protected:
    virtual void OnRender_GL3_3( float t, float dt ) = 0;
    //virtual void OnRender_GL4_6( float t, float dt ) = 0;
    //void OnSpawned() override;
    //void OnDespawned() override;
    //void OnTick( float t, float dt ) override;

    virtual bool OnCreateRenderContext_GL3_3() = 0;
    //virtual bool OnCreateRenderContext_GL4_6() = 0;

    virtual bool OnDestroyRenderContext_GL3_3() = 0;
    //virtual bool OnDestroyRenderContext_GL4_6() = 0;

private:
    bool    mContextInitialized     = false; //here, or in children?
    bool    mDrawingEnabled         = true;
    gl::RenderStage mRenderStage    = gl::RenderStage::STANDARD;

};

} //end of namespace engine
