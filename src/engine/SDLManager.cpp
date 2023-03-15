#include "engine/SDLManager.h"

#include "SDL2/SDL.h"

using namespace engine;

SDLManager::
SDLManager():
    mMainWindowWidth( 0 ),
    mMainWindowHeight( 0 ),
    mMainWindow( nullptr ),
    mMainRenderer( nullptr )
{}


SDLManager::
~SDLManager()
{}


SDL_Window* SDLManager::
GetMainWindow()
{
    return mMainWindow;
}


SDL_Renderer* SDLManager::
GetMainRenderer()
{
    return mMainRenderer;
}


void SDLManager::
SetMainWindow( SDL_Window* window )
{
    mMainWindow = window;
}


void SDLManager::
SetMainRenderer( SDL_Renderer* renderer )
{
    mMainRenderer = renderer;
}


uint32_t SDLManager::
GetMainWindowWidth() const
{
    return mMainWindowWidth;
}


uint32_t SDLManager::
GetMainWindowHeight() const
{
    return mMainWindowHeight;
}


void SDLManager::
SetMainWindowWidth( uint32_t width )
{
    mMainWindowWidth = width;
}


void SDLManager::
SetMainWindowHeight( uint32_t height )
{
    mMainWindowHeight = height;
}



