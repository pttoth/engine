#include "services.h"

using namespace engine;

Services* Services::mInstance = nullptr;

Services::
Services()
{}

Services::
Services(const Services &other)
{}

Services* Services::
instance()
{
    if(nullptr == mInstance){
        mInstance = new Services();
    }
    return mInstance;
}

World *Services::
getWorld()
{
    return instance()->mWorld;
}

void Services::
setWorld(World *world)
{
    instance()->mWorld = world;
}

SDLControl *Services::
getSDLControl()
{
    return instance()->mSdlControl;
}

void Services::
setSDLControl(SDLControl *sdl_control)
{
    instance()->mSdlControl = sdl_control;
}

EngineControl *Services::
getGameControl()
{
    return instance()->mGameControl;
}

void Services::
setGameControl(EngineControl *game_control)
{
    instance()->mGameControl = game_control;
}
