#pragma once

#include "engine/Def.h"
#include "engine/gl/Def.h"
#include "pt/macros.h"
#include "pt/math.h"

#include "engine/gl/UniformBlockFrameInfo.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( DrawingControl )

PT_FORWARD_DECLARE_CLASS( Camera )
PT_FORWARD_DECLARE_CLASS( RealComponent )
PT_FORWARD_DECLARE_CLASS( StandardShaderProgram )

namespace gl{
PT_FORWARD_DECLARE_CLASS( ShaderProgram )
PT_FORWARD_DECLARE_CLASS( Texture2d )
}

class DrawingControl
{
public:
    virtual ~DrawingControl(){}

    virtual void Initialize() = 0;

    //add Drawable to the draw queue
    virtual bool AddDrawable( RealComponent* drawable ) = 0;
    virtual bool RemoveDrawable( RealComponent* drawable ) = 0;

    virtual void ClearCanvas() = 0;
    virtual void DrawScene( float t, float dt ) = 0;

    //@TODO: remove
    virtual void EnableSkybox( bool value ) = 0;

    virtual int32_t GetLightSlot() = 0;
    virtual void    ReleaseLightSlot( int32_t slot ) = 0;

    // return value is 'GLenum' (aka: unsigned int)
    virtual uint32_t GetMainTextureUnit() = 0;
    virtual uint32_t GetNumberOfTextureUnits() const = 0;

    virtual       void      SetCurrentCamera( CameraPtr camera ) = 0;
    virtual const CameraPtr GetCurrentCamera() const = 0;
    virtual       CameraPtr GetCurrentCamera() = 0;

    virtual       void      SetMainCamera( CameraPtr camera ) = 0;
    virtual const CameraPtr GetMainCamera() const = 0;
    virtual       CameraPtr GetMainCamera() = 0;

    virtual gl::Texture2dPtr GetSkyboxTexture() const = 0;
    virtual void             SetSkyboxTexture( const std::string& name ) = 0;

    virtual math::float4    GetClearColor() const = 0;
    virtual void            SetClearColor( float r, float g, float b, float a ) = 0;
    virtual void            SetClearColor( const math::float4& color ) = 0;

    virtual const engine::gl::UniformBlockFrameInfo&   GetUniformBlockFrameInfoRef() const = 0;
    virtual       engine::gl::UniformBlockFrameInfo&   GetUniformBlockFrameInfoRef() = 0;
    virtual uint32_t        GetUniformBlockBindingFrameInfo() = 0;

    //0: wireframes off
    //1: wireframes on, textures on
    //2: wireframes on, textures off
    // @TODO: refactor values (see RendererGL3_3 header)
    virtual int             GetWireframeMode() const = 0;
    virtual void            SetWireframeMode( int val ) = 0;

    virtual bool            GetNormalVectorDisplay() const = 0;
    virtual void            SetNormalVectorDisplay( bool val ) = 0;

    //not sure about this... might be removed later
    // TODO: decide
    virtual void SetCurrentShaderProgram( engine::gl::ShaderProgramPtr pProgram ) = 0;
    virtual engine::gl::ShaderProgramPtr GetCurrentShaderProgram() = 0;

    virtual engine::StandardShaderProgramPtr GetDefaultShaderProgram() = 0;

protected:
private:
};

} //end of namespace engine
