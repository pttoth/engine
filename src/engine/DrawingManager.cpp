#include "engine/DrawingManager.h"

#include "engine/RealComponent.h"

#include "engine/gl/GlWrapper.h"
#include "engine/Services.h"
#include "engine/SDLControl.h"
#include "SDLWrapper.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;

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
    if( !pt::ContainedInVector( mDrawables, drawable ) ){
        mDrawables.push_back( drawable );
        return true;
    }
    pt::log::warn << "DrawingManager::AddDrawable(): " << drawable->GetName() << " is already added!\n";
    assert( false );
    return false;
}


bool DrawingManager::
RemoveDrawable( RealComponent* drawable )
{
    int idx = pt::IndexOfInVector( mDrawables, drawable );
    if( -1 < idx ){
        pt::RemoveElementInVector( mDrawables, idx );
        return true;
    }
    pt::log::warn << "DrawingManager::RemoveDrawable(): Tried to remove '" << drawable->GetName() << "' that is not contained!\n";
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
    //TODO: need to bind OpenGL Context here?
    gl::ClearColor( mClearColor );
    //gl::ClearColor( math::float4() );
    //gl::ClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void DrawingManager::
DrawScene( float t, float dt )
{
    // may not be necessary
    ClearCanvas();

    // render drawables
    for( RealComponent* d : mDrawables ){
        d->Draw( t, dt );
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
SetDefaultShaderProgram( engine::DefaultShaderProgramPtr pProgram )
{
    mShaderProgram = pProgram;
}


engine::DefaultShaderProgramPtr DrawingManager::
GetDefaultShaderProgram()
{
    return mShaderProgram;
}
