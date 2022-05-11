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
GetWorld()
{
    return instance()->mWorld;
}


void Services::
SetWorld(World *world)
{
    instance()->mWorld = world;
}


SDLControl* Services::
GetSDLControl()
{
    return instance()->mSdlControl;
}


void Services::
SetSDLControl(SDLControl *sdl_control)
{
    instance()->mSdlControl = sdl_control;
}


EngineControl* Services::
GetEngineControl()
{
    return instance()->mEngineControl;
}


void Services::
SetEngineControl(EngineControl *engine_control)
{
    instance()->mEngineControl = engine_control;
}


DrawingControl* Services::
GetDrawingControl()
{
    return instance()->mDrawingControl;
}


void Services::
SetDrawingControl(DrawingControl *drawing_control)
{
    instance()->mDrawingControl = drawing_control;
}
