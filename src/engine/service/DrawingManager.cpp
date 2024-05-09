#include "engine/service/DrawingManager.h"

#include "engine/Services.h"
#include "engine/service/AssetControl.h"
#include "engine/service/SDLControl.h"
#include "engine/StandardShaderProgram.h"
#include "engine/component/RealComponent.h"
#include "engine/gl/GlWrapper.h"
#include "engine/gl/ShaderProgram.h"
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

    // draw skybox
        {
        // set MVP matrix to identity
        // set skybox drawing mode in shader (use equirectangular projection)
        // bind skybox texture
        // draw two triangles with [-1,1][-1,1] coordinates
            // (they cover all viewport pixels, projection happens in fragment shader)

            auto dc = Services::GetDrawingControl();
            auto shp = dc->GetDefaultShaderProgram();
            auto cam = dc->GetMainCamera();

            auto skyboxtex = dc->GetSkyboxTexture();
            if( nullptr != skyboxtex ){
                static bool firstTime = true;
                if( firstTime ){
                    skyboxtex->LoadToVRAM();
                }
                skyboxtex->Bind();

                //TODO: make a passthrough vertex shader, use that here
                auto uniSkyboxMode  = shp->GetUniform<int>( "SkyboxMode" );
                auto uniVrot        = shp->GetUniform<mat4>( "Vrot" );
                auto uniP           = shp->GetUniform<mat4>( "P" );

                shp->Use();
                shp->SetUniformModelViewProjectionMatrix( mat4::identity );
                shp->SetUniform( uniSkyboxMode, 1 );
                shp->SetUniform( uniVrot, cam->GetRotationMtx() );
                shp->SetUniform( uniP, cam->GetProjMtx() );

                if( firstTime ){
                    firstTime = false;
                    shp->SetUniform( uniSkyboxMode, 0 );
                    // screen-space coordinates of viewport vertices

                    static std::vector<vec3> vertices = { vec3( -1.0f, 1.0f, 0.0f ),
                                                          vec3( -1.0f, -1.0f, 0.0f ),
                                                          vec3( 1.0f, 1.0f, 0.0f ),
                                                          vec3( -1.0f, -1.0f, 0.0f ),
                                                          vec3( 1.0f, -1.0f, 0.0f ),
                                                          vec3( 1.0f, 1.0f, 0.0f ) };

                    mViewportVertextBuffer = vertices;
                    mViewportVertextBuffer.LoadToVRAM( gl::BufferTarget::ARRAY_BUFFER, gl::BufferHint::STATIC_DRAW );
                }

                gl::BindBuffer( gl::BufferTarget::ARRAY_BUFFER, mViewportVertextBuffer );

                gl::Disable( GL_CULL_FACE );
                gl::EnableVertexAttribArray( 0 );
                gl::VertexAttribPointer( 0, 3,
                                         GL_FLOAT, gl::SKIP_TRANSPOSE,
                                         sizeof(float)*3, 0 );



                //gl::DrawArrays( gl::DrawMode::TRIANGLE_STRIP,
                gl::DrawArrays( gl::DrawMode::TRIANGLES,
                                0,
                                3*3 );

                gl::DisableVertexAttribArray( 0 );

                GL_UnbindBuffer( gl::BufferTarget::ELEMENT_ARRAY_BUFFER );
                GL_UnbindBuffer( gl::BufferTarget::ARRAY_BUFFER );

                shp->SetUniform( uniSkyboxMode, 0 );
            }
        }


    // render drawables normally
    if( 2 != mWireframeMode ){
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

    // draw everything again, in wireframes
    if( 0 != mWireframeMode ){
        gl::Disable( GL_CULL_FACE );
        gl::PolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        auto shp = Services::GetDrawingControl()->GetDefaultShaderProgram();
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
