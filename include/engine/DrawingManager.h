#pragma once

#include "engine/DrawingControl.h"

#include <vector>
#include <memory>

namespace engine{

class DrawingManager;
using DrawingManagerPtr  = std::shared_ptr< DrawingManager >;
using DrawingManagerPtrW = std::weak_ptr< DrawingManager >;

class DrawingManager: public DrawingControl
{
public:
    DrawingManager();
    DrawingManager( const DrawingManager& other ) = delete;
    DrawingManager( DrawingManager&& other ) = delete;
    virtual ~DrawingManager();
    DrawingManager& operator=( const DrawingManager &other )  = delete;
    DrawingManager& operator=( DrawingManager &&other )       = delete;
    bool operator==( const DrawingManager &other ) const      = delete;

    bool AddDrawable( RealComponent* drawable ) override;
    bool RemoveDrawable( RealComponent* drawable ) override;

    void ClearCanvas() override;
    void DrawScene( float t, float dt ) override;
    bool Initialize() override;

    const Camera*   GetMainCamera() const override;
    Camera*         GetMainCamera() override;
    void            SetMainCamera( Camera* camera ) override;

protected:
private:
    std::vector<RealComponent*> mDrawables; //TODO: make this a callqueue
    Camera* mMainCamera = nullptr;
    void* mGLContext; // type is 'SDL_GLContext'

};

} //end of namespace engine
