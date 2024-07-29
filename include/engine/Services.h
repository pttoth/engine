/** -----------------------------------------------------------------------------
  * FILE:    Services.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Service-locator to access various system and engine services.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Def.h"
#include "pt/macros.h"

// @TODO: make threadsafe

namespace engine{

PT_FORWARD_DECLARE_CLASS( AssetControl )
PT_FORWARD_DECLARE_CLASS( DrawingControl )
PT_FORWARD_DECLARE_CLASS( EngineControl )
PT_FORWARD_DECLARE_CLASS( Scheduler )
PT_FORWARD_DECLARE_CLASS( SDLControl )
PT_FORWARD_DECLARE_CLASS( SystemControl )
PT_FORWARD_DECLARE_CLASS( World )

PT_FORWARD_DECLARE_CLASS( Services )

class Services{
    static  ServicesPtr         mInstance;
            AssetControlPtr     mAssetControl = nullptr;
            DrawingControlPtr   mDrawingControl = nullptr;
            EngineControl*      mEngineControl = nullptr;
            SchedulerPtr        mScheduler = nullptr;
            SDLControlPtr       mSdlControl = nullptr;
            SystemControlPtr    mSystemControl = nullptr;
            WorldPtr            mWorld = nullptr;

    Services();
    Services( const Services& other ) = delete;
    Services( Services&& other ) = delete;
    Services& operator=( const Services& other ) = delete;
    Services& operator=( Services&& other ) = delete;
    bool operator==( const Services& other ) const = delete;

public:

    virtual ~Services(){}
    static ServicesPtr  Instance();
    static void         ClearAllServices();

    static AssetControlPtr   GetAssetControl();
    static DrawingControlPtr GetDrawingControl();
    static EngineControl*    GetEngineControl();
    static SchedulerPtr      GetScheduler();
    static SDLControlPtr     GetSDLControl();
    static SystemControlPtr  GetSystemControl();
    static WorldPtr          GetWorld();

    static void SetAssetControl( AssetControlPtr asset_control );
    static void SetDrawingControl( DrawingControlPtr drawing_control );
    static void SetEngineControl( EngineControl* engine_control );
    static void SetScheduler( SchedulerPtr scheduler );
    static void SetSDLControl( SDLControlPtr sdl_control );
    static void SetSystemControl( SystemControlPtr system_control );
    static void SetWorld( WorldPtr world );

};

} //end of namespace 'engine'
