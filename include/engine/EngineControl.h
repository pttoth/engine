/** -----------------------------------------------------------------------------
  * FILE:    EngineControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/Def.h"
#include "pt/config.h"
#include "pt/macros.h"

#include "SDL2/SDL.h"
#include <cstdint>

namespace engine{

PT_FORWARD_DECLARE_CLASS( EngineControl )

class EngineControl
{
public:
    virtual ~EngineControl(){}
    virtual bool        DeveloperMode() const = 0;
    virtual pt::Config  GetConfig() const = 0;
    virtual uint32_t    GetCurrentTime() const = 0;
    virtual SDL_Window* GetMainWindow() = 0;
    virtual std::string ResolveMediaFilePath( const std::string& str ) = 0;


protected:
private:
};

} //end of namespace 'engine'
