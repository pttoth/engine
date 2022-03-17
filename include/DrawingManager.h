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
    virtual void AddDrawable(DrawableComponent* drawable);
    virtual void RemoveDrawable(DrawableComponent* drawable);

    virtual void DrawScene(float t, float dt);

    virtual       void      SetMainCamera(Camera* camera);
    virtual const Camera*   GetMainCamera() const;
    virtual       Camera*   GetMainCamera();

protected:
private:
    std::vector<DrawableComponent*> mDrawables; //TODO: make this a callqueue

    Camera* mMainCamera;
};

}
