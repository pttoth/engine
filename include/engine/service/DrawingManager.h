#pragma once

#include "engine/service/DrawingControl.h"

#include "engine/actor/Camera.h"
#include "engine/gl/Buffer.hpp"
#include "engine/gl/ShaderProgram.h"
#include "engine/gl/Texture2d.h"

#include <vector>

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

    bool AddDrawable( RealComponent* drawable ) override;
    bool RemoveDrawable( RealComponent* drawable ) override;

    void ClearCanvas() override;
    void DrawScene( float t, float dt ) override;

    GLenum GetTextureUnit( const gl::ConstTexture2dPtr tex ) override;

    const CameraPtr GetMainCamera() const override;
    CameraPtr       GetMainCamera() override;
    gl::Texture2dPtr GetSkyboxTexture() const override;

    void            SetMainCamera( CameraPtr camera ) override;

    math::float4    GetClearColor() const override;
    void            SetClearColor( float r, float g, float b, float a ) override;
    void            SetClearColor( const math::float4& color ) override;


    const engine::gl::UniformBlockFrameInfo& GetUniformBlockFrameInfoRef() const override;
          engine::gl::UniformBlockFrameInfo& GetUniformBlockFrameInfoRef() override;

    uint32_t        GetUniformBlockBindingFrameInfo() override;

    void            SetSkyboxTexture( const std::string& name ) override;

    void            SetWireframeMode( int val ) override;

    void SetCurrentShaderProgram( engine::gl::ShaderProgramPtr pProgram ) override;
    void SetDefaultShaderProgram( engine::StandardShaderProgramPtr pProgram );
    //virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram ) override;
    engine::gl::ShaderProgramPtr GetCurrentShaderProgram() override;
    engine::StandardShaderProgramPtr GetDefaultShaderProgram() override;

protected:
    std::vector<RealComponent*>&    GetDrawableGroup( gl::DrawStage drawstage );

private:
    int                             mWireframeMode = 0;
    math::float4                    mClearColor = math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
    engine::gl::ShaderProgramPtr    mCurrentShaderProgram;
    engine::StandardShaderProgramPtr mDefaultShaderProgram;
    gl::Uniform<math::float3>       mUniWireframeColor;
    engine::gl::UniformBlockFrameInfo mUniformFrameInfo;

    std::vector<RealComponent*>     mDrawables; //TODO: make this a callqueue
    std::vector<RealComponent*>     mDrawableGroup_Standard;
    std::vector<RealComponent*>     mDrawableGroup_UIWorld;
    std::vector<RealComponent*>     mDrawableGroup_UIScreen;
    CameraPtr                       mMainCamera;

    gl::Texture2dPtr                mSkyboxTexture;
    gl::Buffer<math::float3>        mViewportVertexBuffer;

};

} //end of namespace engine
