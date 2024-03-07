#pragma once

#include "engine/DrawingControl.h"

#include "engine/Camera.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/gl/Texture.h"

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
    void            SetMainCamera( CameraPtr camera ) override;

    math::float4    GetClearColor() const override;
    void            SetClearColor( float r, float g, float b, float a ) override;
    void            SetClearColor( const math::float4& color ) override;


    virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram );
    //virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram ) override;
    virtual gl::ShaderProgramPtr GetDefaultShaderProgram() override;

protected:
private:
    math::float4                mClearColor = math::float4( 0.0f, 0.0f, 0.0f, 0.0f );
    gl::ShaderProgramPtr        mShaderProgram;
    std::vector<RealComponent*> mDrawables; //TODO: make this a callqueue
    CameraPtr                   mMainCamera;


};

} //end of namespace engine
