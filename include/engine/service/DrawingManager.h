#pragma once

#include "engine/service/DrawingControl.h"

#include "engine/actor/Camera.h"
#include "engine/gl/Buffer.hpp"
#include "engine/gl/ShaderProgram.h"
#include "engine/gl/Texture2d.h"

#include <vector>

// @TODO: 'RealComponent->EnableDraw()' currently prevents drawing meshes in wireframe modes
//          this should only disable world-representation drawing, wireframe is a debug feature

// @TODO: refactor Wireframe mode
//          0: no wireframe
//          1: wireframe
//          2: depth-tested wireframe
//          3: depth-tested wireframe over textures
//          update DrawingControl comments too

namespace engine{

PT_FORWARD_DECLARE_CLASS( DrawingManager )

class DrawingManager: public DrawingControl
{
public:
    DrawingManager();
    DrawingManager( const DrawingManager& other ) = delete;
    DrawingManager( DrawingManager&& other ) = delete;
    virtual ~DrawingManager();
    DrawingManager& operator=( const DrawingManager &other )  = delete;
    DrawingManager& operator=( DrawingManager &&other )       = delete;
    bool operator==( const DrawingManager &other ) const      = delete;

    void Initialize() override;

    bool AddDrawable( RealComponent* drawable ) override;
    bool RemoveDrawable( RealComponent* drawable ) override;

    void ClearCanvas() override;
    void DrawScene( float t, float dt ) override;

    void EnableSkybox( bool value ) override;

    int32_t GetLightSlot() override;
    void    ReleaseLightSlot( int32_t slot ) override;

    uint32_t GetMainTextureUnit() override;
    uint32_t GetNumberOfTextureUnits() const override;

    void            SetCurrentCamera( CameraPtr camera ) override;
    const CameraPtr GetCurrentCamera() const override;
    CameraPtr       GetCurrentCamera() override;

    void            SetMainCamera( CameraPtr camera ) override;
    const CameraPtr GetMainCamera() const override;
    CameraPtr       GetMainCamera() override;
    gl::Texture2dPtr GetSkyboxTexture() const override;



    math::float4    GetClearColor() const override;
    void            SetClearColor( float r, float g, float b, float a ) override;
    void            SetClearColor( const math::float4& color ) override;


    const engine::gl::UniformBlockFrameInfo& GetUniformBlockFrameInfoRef() const override;
          engine::gl::UniformBlockFrameInfo& GetUniformBlockFrameInfoRef() override;

    uint32_t        GetUniformBlockBindingFrameInfo() override;

    void            SetSkyboxTexture( const std::string& name ) override;

    int             GetWireframeMode() const override;
    void            SetWireframeMode( int val ) override;

    bool            GetNormalVectorDisplay() const override;
    void            SetNormalVectorDisplay( bool val ) override;

    void SetCurrentShaderProgram( engine::gl::ShaderProgramPtr pProgram ) override;
    void SetDefaultShaderProgram( engine::StandardShaderProgramPtr pProgram );
    //virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram ) override;
    engine::gl::ShaderProgramPtr GetCurrentShaderProgram() override;
    engine::StandardShaderProgramPtr GetDefaultShaderProgram() override;

protected:
    std::vector<RealComponent*>&    GetDrawableGroup( gl::DrawStage drawstage );
    void                            RenderDrawables( float t, float dt );

private:
    bool                            mInitialized = false;
    uint32_t                        mMaxTextureUnits = 0;
    int                             mWireframeMode = 0;
    bool                            mNormalVectorDisplay = false;
    math::float4                    mClearColor = math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
    engine::gl::ShaderProgramPtr    mCurrentShaderProgram;
    engine::StandardShaderProgramPtr mDefaultShaderProgram;
    gl::Uniform<math::float3>       mUniWireframeColor;
    engine::gl::UniformBlockFrameInfo mUniformFrameInfo;

    bool                            mSkyboxEnabled = true;

    int32_t                         mMaxLightSlot       = 128;
    int32_t                         mNextFreeLightSlot  = 0;

    std::vector<RealComponent*>     mDrawables; //TODO: make this a callqueue
    std::vector<RealComponent*>     mDrawableGroup_Standard;
    std::vector<RealComponent*>     mDrawableGroup_UIWorld;
    std::vector<RealComponent*>     mDrawableGroup_UIScreen;
    CameraPtr                       mMainCamera;

    gl::Texture2dPtr                mSkyboxTexture;
    gl::Buffer<math::float3>        mViewportVertexBuffer;

    //--------------------------------------------------
    //  shadow mapping

    GLuint  mHandleFBO = 0;
    GLuint  mHandleDepthMap = 0; //TODO: remove
    const uint32_t mShadowMapResolution = 1024;

    //--------------------------------------------------

};

} //end of namespace engine
