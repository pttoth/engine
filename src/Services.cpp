#include "Services.h"

using namespace engine;

Services* Services::mInstance = nullptr;

Services::
Services():
    mWorld(nullptr),
    mSdlControl(nullptr),
    mEngineControl(nullptr)
{}


Services::
Services(const Services &other)
{}


Services* Services::
instance()
{
    if(nullptr == mInstance){
        mInstance = new Services();
    }
    return mInstance;
}


World* Services::
getWorld()
{
    return instance()->mWorld;
}


void Services::
setWorld(World *world)
{
    instance()->mWorld = world;
}


SDLControl* Services::
getSDLControl()
{
    return instance()->mSdlControl;
}


void Services::
setSDLControl(SDLControl *sdl_control)
{
    instance()->mSdlControl = sdl_control;
}


EngineControl* Services::
getEngineControl()
{
    return instance()->mEngineControl;
}


void Services::
setGameControl(EngineControl *engine_control)
{
    instance()->mEngineControl = engine_control;
}


DrawingControl* Services::
getDrawingControl()
{
    return instance()->mDrawingControl;
}


void Services::
setDrawingControl(DrawingControl *drawing_control)
{
    instance()->mDrawingControl = drawing_control;
}
