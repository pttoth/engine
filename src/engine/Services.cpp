#include "engine/Services.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

using namespace engine;

ServicesPtr Services::mInstance = nullptr;


Services::
Services()
{
    PT_LOG_DEBUG( "Services instance created." );
}


ServicesPtr Services::Instance()
{
    if( nullptr == mInstance ){
        //mInstance = new Services();
        static std::shared_ptr<Services> instance{ new Services() };
        mInstance = instance;
    }
    return mInstance;
}


DrawingControlPtr Services::
GetDrawingControl()
{
    return Instance()->mDrawingControl;
}


EngineControl* Services::
GetEngineControl()
{
    return Instance()->mEngineControl;
}


SchedulerPtr Services::
GetScheduler()
{
    return Instance()->mScheduler;
}


SDLControlPtr Services::
GetSDLControl()
{
    return Instance()->mSdlControl;
}


SystemControlPtr Services::
GetSystemControl()
{
    return Instance()->mSystemControl;
}


WorldPtr Services::
GetWorld()
{
    return Instance()->mWorld;
}


void Services::
SetDrawingControl( DrawingControlPtr drawing_control )
{
    Instance()->mDrawingControl = drawing_control;
}


void Services::
SetEngineControl( EngineControl* engine_control )
{
    Instance()->mEngineControl = engine_control;
}


void Services::
SetScheduler( SchedulerPtr scheduler )
{
    Instance()->mScheduler = scheduler;
}


void Services::
SetSDLControl( SDLControlPtr sdl_control )
{
    Instance()->mSdlControl = sdl_control;
}


void Services::
SetSystemControl( SystemControlPtr system_control )
{
    Instance()->mSystemControl = system_control;
}


void Services::
SetWorld( WorldPtr world )
{
    Instance()->mWorld = world;
}
