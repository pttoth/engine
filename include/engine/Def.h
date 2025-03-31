/** -----------------------------------------------------------------------------
  * FILE:    Def.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Standalone macros and definitions for project-wide use.
  *          They have no include dependencies, beside "pt/def.h"
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "pt/def.h"

#ifndef ENGINE_DEBUG_NOAUTO
    #ifndef NDEBUG
        #define ENGINE_DEBUG_ENABLED
    #endif
#endif


enum TexComponent{
    DIFFUSE     = 0,
    NORMAL      = 1,
    SPECULAR    = 2,
};
