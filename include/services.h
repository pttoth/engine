/** -----------------------------------------------------------------------------
  * FILE:    services.h
  * AUTHOR:  pttoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Service-locator to access various system and engine services
  * -----------------------------------------------------------------------------
  */

#pragma once

namespace engine{

class World;
class SDLControl;
class EngineControl;

class Services{
    static Services*      mInstance;
           World*         mWorld;
           SDLControl*    mSdlControl;
           EngineControl* mGameControl;

    Services();
    Services(const Services& other);
    Services(Services&& other);
    virtual ~Services(){}
    Services& operator=(const Services &other);
    Services& operator=(Services &&other);
    bool operator==(const Services &other) const;

public:
    static Services* instance();

    static World* getWorld();
    static void setWorld(World* world);
    static SDLControl* getSDLControl();
    static void setSDLControl(SDLControl* sdl_control);
    static EngineControl* getGameControl();
    static void setGameControl(EngineControl* game_control);
};

}
