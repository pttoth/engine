#include "Services.h"

using namespace engine;

Services* Services::mInstance = nullptr;

Services::
Services():
    mWorld(nullptr),
    mSdlControl(nullptr),
    mDrawingControl(nullptr),
    mScheduler(nullptr)
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


SDLControl* Services::
GetSDLControl()
{
    return instance()->mSdlControl;
}


DrawingControl* Services::
GetDrawingControl()
{
    return instance()->mDrawingControl;
}


Scheduler* Services::GetScheduler()
{
    return instance()->mScheduler;
}


void Services::
SetWorld(World *world)
{
    instance()->mWorld = world;
}


void Services::
SetSDLControl(SDLControl *sdl_control)
{
    instance()->mSdlControl = sdl_control;
}


void Services::
SetDrawingControl(DrawingControl *drawing_control)
{
    instance()->mDrawingControl = drawing_control;
}


void Services::SetScheduler(Scheduler *scheduler)
{
    instance()->mScheduler = scheduler;
}
