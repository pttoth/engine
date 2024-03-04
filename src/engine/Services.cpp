#include "engine/Services.h"

#include "pt/utility.hpp"

using namespace engine;

ServicesPtr Services::mInstance = nullptr;

Services::
Services()
{}


Services::
Services( const Services& other )
{}


Services::
~Services()
{}


ServicesPtr Services::
Instance()
{
    if( nullptr == mInstance ){
        mInstance = NewPtr<Services>();
    }
    return mInstance;
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


Scheduler* Services::
GetScheduler()
{
    return Instance()->mScheduler;
}


SDLControl* Services::
GetSDLControl()
{
    return Instance()->mSdlControl;
}


SystemControl* Services::
GetSystemControl()
{
    return Instance()->mSystemControl;
}


World* Services::
GetWorld()
{
    return Instance()->mWorld;
}


void Services::
SetDrawingControl( DrawingControl* drawing_control )
{
    Instance()->mDrawingControl = drawing_control;
}


void Services::
SetEngineControl( EngineControl* engine_control )
{
    Instance()->mEngineControl = engine_control;
}


void Services::
SetScheduler( Scheduler* scheduler )
{
    Instance()->mScheduler = scheduler;
}


void Services::
SetSDLControl( SDLControl* sdl_control )
{
    Instance()->mSdlControl = sdl_control;
}


void Services::
SetSystemControl( SystemControl* system_control )
{
    Instance()->mSystemControl = system_control;
}


void Services::
SetWorld( World* world )
{
    Instance()->mWorld = world;
}
