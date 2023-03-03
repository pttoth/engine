#pragma once

#include "engine/experimental/DrawingControl.h"

#include <vector>
#include <memory>

namespace engine{
namespace experimental{

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

    virtual bool AddDrawable( RealComponent* drawable ) override;
    virtual bool RemoveDrawable( RealComponent* drawable ) override;

    virtual void DrawScene( float t, float dt ) override;
    virtual void ClearCanvas() override;

    virtual       void      SetMainCamera( Camera* camera ) override;
    virtual const Camera*   GetMainCamera() const override;
    virtual       Camera*   GetMainCamera() override;

protected:
private:
    std::vector<RealComponent*> mDrawables; //TODO: make this a callqueue
    Camera* mMainCamera = nullptr;
};

} //end of namespace experimental
} //end of namespace engine
