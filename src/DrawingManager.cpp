#include "DrawingManager.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include "DrawableComponent.h"

#include "Services.h"
#include "SDLControl.h"

using namespace engine;


DrawingManager::
DrawingManager():
    mMainCamera(nullptr)
{}


DrawingManager::
~DrawingManager()
{}


void DrawingManager::
AddDrawable(DrawableComponent* drawable)
{
    if(pt::ContainedInVector(mDrawables, drawable)){
        pt::log::warn << "DrawingManager::AddDrawable(): " << drawable->GetName() << " is already added!\n";
    }else{
        mDrawables.push_back(drawable);
    }
}


void DrawingManager::
RemoveDrawable(DrawableComponent* drawable)
{
    int idx = pt::IndexOfInVector(mDrawables, drawable);
    if( -1 < idx ){
        pt::RemoveElementInVector(mDrawables, idx);
    }else{
        pt::log::warn << "DrawingManager::RemoveDrawable(): Tried to remove '" << drawable->GetName() << "' that is not contained!\n";
    }
}


void DrawingManager::
DrawScene(float t, float dt)
{
    auto sdlc = Services::getSDLControl();
    auto r = sdlc->GetMainRenderer();

    //not necessary
    ClearCanvas();

    for(DrawableComponent* d : mDrawables){
        d->Draw(t,dt);
    }

    sdlc->RenderPresent(r);
}

void DrawingManager::
ClearCanvas()
{
    auto sdlc = Services::getSDLControl();
    auto r = sdlc->GetMainRenderer();

    sdlc->SetRenderDrawColor(r, 0,0,0,255);
    sdlc->RenderClear(r);
}


void DrawingManager::
SetMainCamera(Camera *camera)
{
    mMainCamera = camera;
}


const Camera *DrawingManager::
GetMainCamera() const
{
    return mMainCamera;
}


Camera *DrawingManager::
GetMainCamera()
{
    return mMainCamera;
}
