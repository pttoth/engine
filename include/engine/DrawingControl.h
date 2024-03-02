#pragma once

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Camera )
PT_FORWARD_DECLARE_CLASS( RealComponent )
namespace gl{
PT_FORWARD_DECLARE_CLASS( ShaderProgram )
}

class DrawingControl
{
public:
    //add Drawable to the draw queue
    virtual bool AddDrawable( RealComponent* drawable ) = 0;
    virtual bool RemoveDrawable( RealComponent* drawable ) = 0;

    virtual void ClearCanvas() = 0;
    virtual void DrawScene( float t, float dt ) = 0;
    virtual bool Initialize() = 0;

    virtual       void      SetMainCamera( CameraPtr camera ) = 0;
    virtual const CameraPtr GetMainCamera() const = 0;
    virtual       CameraPtr GetMainCamera() = 0;

    //virtual void SetDefaultShaderProgram( gl::ShaderProgramPtr pProgram ) = 0;
    virtual gl::ShaderProgramPtr GetDefaultShaderProgram() = 0;

protected:
private:
};

} //end of namespace engine
