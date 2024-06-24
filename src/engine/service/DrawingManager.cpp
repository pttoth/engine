#include "engine/service/DrawingManager.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/SDLControl.h"
#include "engine/StandardShaderProgram.h"
#include "engine/component/RealComponent.h"
#include "engine/gl/GlWrapper.h"
#include "engine/gl/ShaderProgram.h"
#include "engine/gl/UniformBlockFrameInfo.h"
#include "SDLWrapper.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace math;

using namespace pt;

DrawingManager::
DrawingManager()
{}


DrawingManager::
~DrawingManager()
{}


bool DrawingManager::
AddDrawable( RealComponent* drawable )
{
    if( nullptr == drawable ){
#ifdef PT_DEBUG_ENABLED
        pt::PrintStackTrace( "'nullptr' supplied as 'drawable'" );
#endif
        return false;
    }
    if( !pt::ContainedInVector( mDrawables, drawable ) ){
        mDrawables.push_back( drawable );
        auto& drawableGroup = GetDrawableGroup( drawable->GetPreferredDrawStage() );
        drawableGroup.push_back( drawable );
        return true;
    }
    PT_LOG_WARN( "DrawingManager::AddDrawable(): " << drawable->GetName() << " is already added!" );
    assert( false );
    return false;
}


bool DrawingManager::
RemoveDrawable( RealComponent* drawable )
{
    if( nullptr == drawable ){
#ifdef PT_DEBUG_ENABLED
        pt::PrintStackTrace( "'nullptr' supplied as 'drawable'" );
#endif
        return false;
    }
    int idx = pt::IndexOfInVector( mDrawables, drawable );
    if( -1 < idx ){
        pt::RemoveElementInVector( mDrawables, idx );
        auto& drawableGroup = GetDrawableGroup( drawable->GetPreferredDrawStage() );
        int idx_group = pt::IndexOfInVector( drawableGroup, drawable );
        if( -1 < idx_group ){
            pt::RemoveElementInVector( drawableGroup, idx_group );
        }
        return true;
    }
    PT_LOG_WARN( "DrawingManager::RemoveDrawable(): Tried to remove '" << drawable->GetName() << "' that is not contained!" );
    return false;
}


void DrawingManager::
ClearCanvas()
{
    // SDL
    //auto sdlc = Services::GetSDLControl();
    //auto r = sdlc->GetMainRenderer();
    //sdl::SetRenderDrawColor( r, 0, 0, 0, 255 );
    //sdl::RenderClear( r );

    // OpenGL
    gl::ClearColor( mClearColor );
    gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void DrawingManager::
DrawScene( float t, float dt )
{
    // may not be necessary
    ClearCanvas();

    auto shp = mDefaultShaderProgram;
    auto cam = mMainCamera;

    // update shared uniforms
    mUniformFrameInfo.SetT( t );
    mUniformFrameInfo.SetDT( dt );
    mUniformFrameInfo.SetV( cam->GetViewMtx() );
    mUniformFrameInfo.SetP( cam->GetProjMtx() );
    mUniformFrameInfo.SetVrot( cam->GetLookAtMtx() );
    mUniformFrameInfo.SetPV( cam->GetProjMtx() * cam->GetViewMtx() );
    mUniformFrameInfo.SetPVrotInv( cam->GetLookAtMtx().invert() * cam->GetProjMtx().invert() );

    mUniformFrameInfo.LoadToVRAM( gl::BufferTarget::UNIFORM_BUFFER, gl::BufferHint::STREAM_DRAW );
    mUniformFrameInfo.BindBufferToBindingPoint( 0 );


    // draw skybox
        {
        // draw two triangles with [-1,1][-1,1] coordinates
            // (they cover all viewport pixels, projection happens in fragment shader)

            auto skyboxtex = this->GetSkyboxTexture();
            if( nullptr != skyboxtex ){
                skyboxtex->Bind();

                //TODO: make a passthrough vertex shader, use that here
                auto uniSkyboxMode  = shp->GetUniform<int>( "SkyboxMode" );

                shp->Use();
                shp->SetUniformModelViewProjectionMatrix( mat4::identity );
                shp->SetUniform( uniSkyboxMode, 1 );

                static bool firstTime = true;
                if( firstTime ){
                    firstTime = false;
                    // screen-space coordinates of viewport vertices

                    static std::vector<vec3> vertices = { vec3( -1.0f, 1.0f, 0.0f ),
                                                          vec3( -1.0f, -1.0f, 0.0f ),
                                                          vec3( 1.0f, 1.0f, 0.0f ),
                                                          vec3( 1.0f, -1.0f, 0.0f ) };

                    mViewportVertexBuffer = vertices;
                    mViewportVertexBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
                }

                gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mViewportVertexBuffer );

                gl::EnableVertexAttribArray( 0 );
                gl::VertexAttribPointer( 0, 3,
                                         GL_FLOAT, gl::SKIP_TRANSPOSE,
                                         sizeof(float)*3, 0 );

                gl::DrawArrays( gl::DrawMode::TRIANGLE_STRIP, 0, 3+1 );

                gl::DisableVertexAttribArray( 0 );

                PT_GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
                PT_GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );

                shp->SetUniform( uniSkyboxMode, 0 );
            }
        }

    // -------------------------
    // render lights' shadow maps
    // -------------------------
    // create FBO
    // create depth cubemap
    // set up FBO

    // for every light
    //
    // -------------------------

    // render drawables normally
    if( 2 != mWireframeMode ){
        RenderDrawables( t, dt );
    }

    // draw everything again, in wireframes
    if( 0 != mWireframeMode ){
        gl::Disable( GL_CULL_FACE );
        gl::PolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        auto uniWF = shp->GetUniform<int>( "WireframeMode" );
        auto uni = shp->GetUniform<vec3>( "WireframeColor" );

        shp->SetUniform( uniWF, 1 );
        shp->SetUniform( uni, vec3::cyan );

        for( RealComponent* d : mDrawables ){
            if( d->IsDrawEnabled() ){
                d->Draw( t, dt );
            }
        }

        shp->SetUniform( uniWF, 0 );

        gl::Enable( GL_CULL_FACE );
        gl::PolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }


    // OpenGL
    auto sdlc = Services::GetSDLControl();
    auto window = sdlc->GetMainWindow();
    sdl::GL_SwapWindow( window );

    //-----
    // SDL
    //auto r = sdlc->GetMainRenderer();
    //sdl::RenderPresent(r);
}


GLenum DrawingManager::
GetTextureUnit( const gl::ConstTexture2dPtr tex )
{
    // TODO: implement texturesampler selector here
    return GL_TEXTURE0;
}


const CameraPtr DrawingManager::
GetMainCamera() const
{
    return mMainCamera;
}


CameraPtr DrawingManager::
GetMainCamera()
{
    return mMainCamera;
}


gl::Texture2dPtr DrawingManager::
GetSkyboxTexture() const
{
    return mSkyboxTexture;
}


void DrawingManager::
SetMainCamera( CameraPtr camera )
{
    mMainCamera = camera;
}


math::float4 DrawingManager::
GetClearColor() const
{
    return mClearColor;
}


void DrawingManager::
SetClearColor( float r, float g, float b, float a )
{
    mClearColor = math::float4( r, g, b, a );
}


void DrawingManager::
SetClearColor( const math::float4& color )
{
    mClearColor = color;
}


const gl::UniformBlockFrameInfo& DrawingManager::
GetUniformBlockFrameInfoRef() const
{
    return mUniformFrameInfo;
}


gl::UniformBlockFrameInfo& DrawingManager::
GetUniformBlockFrameInfoRef()
{
    return mUniformFrameInfo;
}


uint32_t DrawingManager::
GetUniformBlockBindingFrameInfo()
{
    return 0;
}


void DrawingManager::
SetSkyboxTexture( const std::string& name )
{
    auto ac = Services::GetAssetControl();
    if( nullptr != ac ){
        gl::Texture2dPtr ptex = ac->GetTexture( name );
        mSkyboxTexture = ptex;
        if( nullptr != ptex && !ptex->IsLoadedInVRAM() ){
            ptex->LoadToVRAM();
        }
    }
}


void DrawingManager::
SetWireframeMode( int val )
{
    if( 0 <= val && val <= 2 ){
        mWireframeMode = val;
    }else{
        PT_LOG_ERR( "Tried to set invalid wireframe mode (" << val << ")." );
    }
}


void DrawingManager::
SetCurrentShaderProgram( engine::gl::ShaderProgramPtr pProgram )
{
    mCurrentShaderProgram = pProgram;
}


void DrawingManager::
SetDefaultShaderProgram( engine::StandardShaderProgramPtr pProgram )
{
    mDefaultShaderProgram = pProgram;
}


engine::gl::ShaderProgramPtr DrawingManager::
GetCurrentShaderProgram()
{
    return mCurrentShaderProgram;
}


engine::StandardShaderProgramPtr DrawingManager::
GetDefaultShaderProgram()
{
    return mDefaultShaderProgram;
}


std::vector<RealComponent*>& DrawingManager::
GetDrawableGroup( gl::DrawStage drawstage )
{
    switch( drawstage ){
    case gl::DrawStage::STANDARD:
        return mDrawableGroup_Standard;
    case gl::DrawStage::UI_WORLD:
        return mDrawableGroup_UIWorld;
    case gl::DrawStage::UI_SCREEN:
        return mDrawableGroup_UIScreen;
    default:
        return mDrawableGroup_Standard;
    }
}


void DrawingManager::
RenderDrawables( float t, float dt )
{
    for( RealComponent* d : mDrawableGroup_Standard ){
        if( d->IsDrawEnabled() ){
            d->Draw( t, dt );
        }
    }
    for( RealComponent* d : mDrawableGroup_UIWorld ){
        if( d->IsDrawEnabled() ){
            d->Draw( t, dt );
        }
    }
    for( RealComponent* d : mDrawableGroup_UIScreen ){
        if( d->IsDrawEnabled() ){
            d->Draw( t, dt );
        }
    }
}
