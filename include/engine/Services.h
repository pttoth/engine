/** -----------------------------------------------------------------------------
  * FILE:    Services.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Service-locator to access various system and engine services.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( World )
PT_FORWARD_DECLARE_CLASS( SDLControl )
PT_FORWARD_DECLARE_CLASS( DrawingControl )
PT_FORWARD_DECLARE_CLASS( Scheduler )

class Services{
    static  Services*           mInstance;
            World*              mWorld = nullptr;
            SDLControl*         mSdlControl = nullptr;
            DrawingControl*     mDrawingControl = nullptr;
            Scheduler*          mScheduler = nullptr;

    Services();
    Services(const Services& other);
    Services(Services&& other);
    virtual ~Services();
    Services& operator=(const Services &other);
    Services& operator=(Services &&other);
    bool operator==(const Services &other) const;

public:
    static Services* Instance();

    static World*           GetWorld();
    static SDLControl*      GetSDLControl();
    static DrawingControl*  GetDrawingControl();
    static Scheduler*       GetScheduler();

    static void SetWorld(World* world);
    static void SetSDLControl(SDLControl* sdl_control);
    static void SetDrawingControl(DrawingControl* drawing_control);
    static void SetScheduler(Scheduler* scheduler);
};

} //end of namespace engine
