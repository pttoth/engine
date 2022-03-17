#include "DrawingManager.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include "DrawableComponent.h"

using namespace engine;


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
    for(DrawableComponent* d : mDrawables){
        d->Draw(0,0); //TODO: time and delta
    }
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
