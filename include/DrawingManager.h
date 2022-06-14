/** -----------------------------------------------------------------------------
  * FILE:    DrawingManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE:
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "DrawingControl.h"

#include<vector>

namespace engine{

class Camera;
class DrawableComponent;

class DrawingManager: public DrawingControl{

public:
    DrawingManager();
    DrawingManager(const DrawingManager& other) = delete;
    DrawingManager(DrawingManager&& other) = delete;
    virtual ~DrawingManager();
    DrawingManager& operator=(const DrawingManager &other)  = delete;
    DrawingManager& operator=(DrawingManager &&other)       = delete;
    bool operator==(const DrawingManager &other) const      = delete;

    //add Drawable to the draw queue
    virtual void AddDrawable(DrawableComponent* drawable) override;
    virtual void RemoveDrawable(DrawableComponent* drawable) override;

    virtual void DrawScene(float t, float dt) override;
    virtual void ClearCanvas() override;

    virtual       void      SetMainCamera(Camera* camera) override;
    virtual const Camera*   GetMainCamera() const override;
    virtual       Camera*   GetMainCamera() override;

protected:
private:
    std::vector<DrawableComponent*> mDrawables; //TODO: make this a callqueue

    Camera* mMainCamera = nullptr;
};

}
