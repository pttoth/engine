/** -----------------------------------------------------------------------------
  * FILE:    Services.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Service-locator to access various system and engine services
  * -----------------------------------------------------------------------------
  */

#pragma once

namespace engine{

class World;
class SDLControl;
class DrawingControl;
class Scheduler;


class Services{
    static  Services*           mInstance;
            World*              mWorld;
            SDLControl*         mSdlControl;
            DrawingControl*     mDrawingControl;
            Scheduler*          mScheduler;

    Services();
    Services(const Services& other);
    Services(Services&& other);
    virtual ~Services(){}
    Services& operator=(const Services &other);
    Services& operator=(Services &&other);
    bool operator==(const Services &other) const;

public:
    static Services* instance();

    static World* GetWorld();
    static void SetWorld(World* world);
    static SDLControl* GetSDLControl();
    static void SetSDLControl(SDLControl* sdl_control);
    static DrawingControl* GetDrawingControl();
    static void SetDrawingControl(DrawingControl* drawing_control);
    static Scheduler* GetScheduler();
    static void SetScheduler(Scheduler* scheduler);
};

}
