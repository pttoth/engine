#include "engine/Services.h"

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


EngineControl* Services::
GetEngineControl()
{
    return Instance()->mEngineControl;
}


Scheduler* Services::GetScheduler()
{
    return Instance()->mScheduler;
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
SetEngineControl( EngineControl* engine_control )
{
    Instance()->mEngineControl = engine_control;
}


void Services::
SetScheduler(Scheduler *scheduler)
{
    Instance()->mScheduler = scheduler;
}
