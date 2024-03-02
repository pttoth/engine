#pragma once

#include "engine/DrawingControl.h"

#include "engine/Camera.h"
#include "engine/gl/ShaderProgram.h"
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
    bool Initialize() override;

    const CameraPtr GetMainCamera() const override;
    CameraPtr       GetMainCamera() override;
    void            SetMainCamera( CameraPtr camera ) override;

    virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram );
    //virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram ) override;
    virtual gl::ShaderProgramPtr GetDefaultShaderProgram() override;

protected:
private:
    gl::ShaderProgramPtr        mShaderProgram;
    std::vector<RealComponent*> mDrawables; //TODO: make this a callqueue
    CameraPtr                   mMainCamera;
    void*                       mGLContext; // type is 'SDL_GLContext'

};

} //end of namespace engine
