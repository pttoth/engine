/** -----------------------------------------------------------------------------
  * FILE:    DrawingManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include<vector>

namespace engine{

class Camera;
class DrawableComponent;

class DrawingManager{

public:
    //add Drawable to the draw queue
    virtual void AddDrawable(DrawableComponent* drawable) = 0;
    virtual void RemoveDrawable(DrawableComponent* drawable) = 0;

    virtual void DrawScene() = 0;

    virtual       void      SetMainCamera(Camera* camera) = 0;
    virtual const Camera*   GetMainCamera() const = 0;
    virtual       Camera*   GetMainCamera() = 0;

protected:
private:
    std::vector<DrawableComponent*> mDrawables;
};

}
