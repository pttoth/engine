#include "DrawingManager.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include "DrawableComponent.h"

using namespace engine;


void DrawingManager::AddDrawable(DrawableComponent* drawable)
{
    if(pt::ContainedInVector(mDrawables, drawable)){
        pt::log::warn << "DrawingManager::AddDrawable(): " << "" << "drawable.Get is already added!\n";
    }else{
        mDrawables.push_back(drawable);
    }
}

void DrawingManager::RemoveDrawable(DrawableComponent* drawable)
{

}

void DrawingManager::DrawScene()
{

}

void DrawingManager::SetMainCamera(Camera *camera)
{

}

const Camera *DrawingManager::GetMainCamera() const
{

}

Camera *DrawingManager::GetMainCamera()
{

}
