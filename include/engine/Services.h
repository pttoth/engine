/** -----------------------------------------------------------------------------
  * FILE:    Services.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Service-locator to access various system and engine services.
  * -----------------------------------------------------------------------------
  */

#pragma once

namespace engine{

class World;
class SDLControl;
class DrawingControl;
class Scheduler;

//TODO: remove namespace
namespace experimental{
    class Scheduler;
    class DrawingControl;
}

class Services{
    static  Services*           mInstance;
            World*              mWorld = nullptr;
            SDLControl*         mSdlControl = nullptr;
            DrawingControl*     mDrawingControl = nullptr;
            experimental::DrawingControl*     mDrawingControl2 = nullptr;
            Scheduler*          mScheduler = nullptr;
            experimental::Scheduler* mScheduler2 = nullptr;

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
    static experimental::DrawingControl*  GetDrawingControl2();
    static Scheduler*       GetScheduler();     //TODO: remove this
    static experimental::Scheduler*  GetScheduler2();  //TODO: reNAME this

    static void SetWorld(World* world);
    static void SetSDLControl(SDLControl* sdl_control);
    static void SetDrawingControl(DrawingControl* drawing_control);
    static void SetDrawingControl2(experimental::DrawingControl* drawing_control);
    static void SetScheduler(Scheduler* scheduler);
    static void SetScheduler2(experimental::Scheduler* scheduler);
};

}
