#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Camera )
PT_FORWARD_DECLARE_CLASS( RealComponent )
PT_FORWARD_DECLARE_CLASS( DefaultShaderProgram )

namespace gl{
PT_FORWARD_DECLARE_CLASS( ShaderProgram )
PT_FORWARD_DECLARE_CLASS( Texture2d )
}

class DrawingControl
{
public:
    //add Drawable to the draw queue
    virtual bool AddDrawable( RealComponent* drawable ) = 0;
    virtual bool RemoveDrawable( RealComponent* drawable ) = 0;

    virtual void ClearCanvas() = 0;
    virtual void DrawScene( float t, float dt ) = 0;

    // return value is 'GLenum' (aka: unsigned int)
    virtual uint32_t GetTextureUnit( const gl::ConstTexture2dPtr tex ) = 0;

    virtual       void      SetMainCamera( CameraPtr camera ) = 0;
    virtual const CameraPtr GetMainCamera() const = 0;
    virtual       CameraPtr GetMainCamera() = 0;

    virtual math::float4    GetClearColor() const = 0;
    virtual void            SetClearColor( float r, float g, float b, float a ) = 0;
    virtual void            SetClearColor( const math::float4& color ) = 0;

    //0: wireframes off
    //1: wireframes on, textures on
    //2: wireframes on, textures off
    virtual void            SetWireframeMode( int val ) = 0;

    //not sure about this... might be removed later
    // TODO: decide
    virtual void SetCurrentShaderProgram( engine::gl::ShaderProgramPtr pProgram ) = 0;
    virtual engine::gl::ShaderProgramPtr GetCurrentShaderProgram() = 0;

    virtual engine::DefaultShaderProgramPtr GetDefaultShaderProgram() = 0;

protected:
private:
};

} //end of namespace engine
