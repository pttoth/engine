#pragma once

namespace engine{
class Camera;

class RealComponent;

class DrawingControl
{
public:
    //add Drawable to the draw queue
    virtual bool AddDrawable( RealComponent* drawable ) = 0;
    virtual bool RemoveDrawable( RealComponent* drawable ) = 0;

    virtual void DrawScene( float t, float dt ) = 0;
    virtual void ClearCanvas() = 0;

    virtual       void      SetMainCamera( Camera* camera ) = 0;
    virtual const Camera*   GetMainCamera() const = 0;
    virtual       Camera*   GetMainCamera() = 0;

protected:
private:
};

} //end of namespace engine
