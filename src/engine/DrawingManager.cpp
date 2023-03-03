#include "engine/experimental/DrawingManager.h"

#include "engine/experimental/RealComponent.h"

#include "engine/Services.h"
#include "engine/SDLControl.h"
#include "GlWrapper.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

using namespace pt;


DrawingManager::
DrawingManager()
{}


DrawingManager::
~DrawingManager()
{}


bool DrawingManager::
AddDrawable( experimental::RealComponent* drawable )
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
RemoveDrawable( experimental::RealComponent* drawable )
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
DrawScene( float t, float dt )
{
    // OpenGL
    auto sdlc = Services::GetSDLControl();
    auto window = sdlc->GetMainWindow();

    // SDL only
    auto r = sdlc->GetMainRenderer();

    // may not be necessary
    ClearCanvas();

    // render drawables
    for( RealComponent* d : mDrawables ){
        d->Draw( t, dt );
    }

    // update screen
    //sdlc->GL_SwapWindow( window );
    sdlc->RenderPresent(r);
}

void DrawingManager::
ClearCanvas()
{
    auto sdlc = Services::GetSDLControl();
    auto r = sdlc->GetMainRenderer();

    sdlc->SetRenderDrawColor( r, 0, 0, 0, 255 );
    sdlc->RenderClear( r );

    //TODO: need to bind OpenGL Context here?
    //gl::ClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    //gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void DrawingManager::
SetMainCamera( Camera *camera )
{
    mMainCamera = camera;
}


const Camera* DrawingManager::
GetMainCamera() const
{
    return mMainCamera;
}


Camera* DrawingManager::
GetMainCamera()
{
    return mMainCamera;
}
