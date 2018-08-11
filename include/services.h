/** -----------------------------------------------------------------------------
  * FILE:    services.h
  * AUTHOR:  pttoth - 2018.07.11.
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Built upon the Service-Locator pattern, this singleton is used
  *            to locate any service the caller wants to access
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "gamecontrol.h"

namespace pttoth{
namespace engine{

class World;
class SDLControl;
class GameControl;

class Services{
    static Services*    _instance;
           World*       _world;
           SDLControl*  _sdl_control;
           GameControl* _game_control;

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
    static GameControl* getGameControl();
    static void setGameControl(GameControl* game_control);
};

} }
