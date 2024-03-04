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

namespace engine{

PT_FORWARD_DECLARE_CLASS( Services )

PT_FORWARD_DECLARE_CLASS( DrawingControl )
PT_FORWARD_DECLARE_CLASS( EngineControl )
PT_FORWARD_DECLARE_CLASS( Scheduler )
PT_FORWARD_DECLARE_CLASS( SDLControl )
PT_FORWARD_DECLARE_CLASS( SystemControl )
PT_FORWARD_DECLARE_CLASS( World )

class Services{
    static  ServicesPtr         mInstance;
            DrawingControl*     mDrawingControl = nullptr;
            EngineControl*      mEngineControl = nullptr;
            Scheduler*          mScheduler = nullptr;
            SDLControl*         mSdlControl = nullptr;
            SystemControl*      mSystemControl = nullptr;
            World*              mWorld = nullptr;

    Services();
    Services( const Services& other );
    Services( Services&& other );
    virtual ~Services();
    Services& operator=( const Services& other );
    Services& operator=( Services&& other );
    bool operator==( const Services& other ) const;

public:
    static ServicesPtr Instance();

    static DrawingControl*  GetDrawingControl();
    static EngineControl*   GetEngineControl();
    static Scheduler*       GetScheduler();
    static SDLControl*      GetSDLControl();
    static SystemControl*   GetSystemControl();
    static World*           GetWorld();

    static void SetDrawingControl( DrawingControl* drawing_control );
    static void SetEngineControl( EngineControl* engine_control );
    static void SetScheduler( Scheduler* scheduler );
    static void SetSDLControl( SDLControl* sdl_control );
    static void SetSystemControl( SystemControl* system_control );
    static void SetWorld( World* world );

};

} //end of namespace engine
