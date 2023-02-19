#include "engine/Services.h"

using namespace engine;

Services* Services::mInstance = nullptr;

Services::
Services():
    mWorld(nullptr),
    mSdlControl(nullptr),
    mDrawingControl(nullptr),
    mScheduler(nullptr),
    mScheduler2(nullptr)
{}


Services::
Services(const Services &other)
{}


Services::
~Services()
{}


Services* Services::
Instance()
{
    if(nullptr == mInstance){
        mInstance = new Services();
    }
    return mInstance;
}


World* Services::
GetWorld()
{
    return Instance()->mWorld;
}


SDLControl* Services::
GetSDLControl()
{
    return Instance()->mSdlControl;
}


DrawingControl* Services::
GetDrawingControl()
{
    return Instance()->mDrawingControl;
}


experimental::DrawingControl* Services::
GetDrawingControl2()
{
    return Instance()->mDrawingControl2;
}


Scheduler* Services::GetScheduler()
{
    return Instance()->mScheduler;
}


experimental::Scheduler *Services::
GetScheduler2()
{
    return Instance()->mScheduler2;
}


void Services::
SetWorld(World *world)
{
    Instance()->mWorld = world;
}


void Services::
SetSDLControl(SDLControl *sdl_control)
{
    Instance()->mSdlControl = sdl_control;
}


void Services::
SetDrawingControl(DrawingControl *drawing_control)
{
    Instance()->mDrawingControl = drawing_control;
}


void Services::
SetDrawingControl2(experimental::DrawingControl *drawing_control)
{
    Instance()->mDrawingControl2 = drawing_control;
}


void Services::
SetScheduler(Scheduler *scheduler)
{
    Instance()->mScheduler = scheduler;
}


void Services::
SetScheduler2( experimental::Scheduler* scheduler )
{
    Instance()->mScheduler2 = scheduler;
}
