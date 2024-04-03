#include "engine/Services.h"

#include "pt/utility.hpp"
#include "pt/logging.h"

#include <assert.h>

using namespace engine;

ServicesPtr Services::mInstance = nullptr;

Services::
Services()
{
    PT_LOG_DEBUG( "Services instance created." );
}


ServicesPtr Services::
Instance()
{
    if( nullptr == mInstance ){
        //mInstance = new Services();
        static std::shared_ptr<Services> instance{ new Services() };
        mInstance = instance;
    }
    return mInstance;
}


AssetControlPtr Services::
GetAssetControl()
{
    assert( nullptr != Instance()->mAssetControl );
    return Instance()->mAssetControl;
}


DrawingControlPtr Services::
GetDrawingControl()
{
    assert( nullptr != Instance()->mDrawingControl );
    return Instance()->mDrawingControl;
}


EngineControl* Services::
GetEngineControl()
{
    assert( nullptr != Instance()->mEngineControl );
    return Instance()->mEngineControl;
}


SchedulerPtr Services::
GetScheduler()
{
    assert( nullptr != Instance()->mScheduler );
    return Instance()->mScheduler;
}


SDLControlPtr Services::
GetSDLControl()
{
    assert( nullptr != Instance()->mSdlControl );
    return Instance()->mSdlControl;
}


SystemControlPtr Services::
GetSystemControl()
{
    assert( nullptr != Instance()->mSystemControl );
    return Instance()->mSystemControl;
}


WorldPtr Services::
GetWorld()
{
    assert( nullptr != Instance()->mWorld );
    return Instance()->mWorld;
}


void Services::
SetAssetControl( AssetControlPtr asset_control )
{
    Instance()->mAssetControl = asset_control;
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
