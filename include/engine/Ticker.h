/** -----------------------------------------------------------------------------
  * FILE:    Ticker.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * PURPOSE: Interface for classes that are updated by the engine timer.
  * -----------------------------------------------------------------------------
  */

#pragma once

#include "engine/Def.h"

namespace engine
{

class Ticker
{
public:

    enum class Group{
        NO_GROUP = 0, //TODO: probably needs deletion
        PREPHYSICS,
        DURINGPHYSICS,
        POSTPHYSICS,
    };


    virtual float GetTickInterval() const = 0;
    virtual Group GetTickGroup() const = 0;

    virtual bool IsTickEnabled() const = 0;
    virtual bool IsTickRegistered() const = 0;

    virtual void Tick( float t, float dt ) = 0;

};

} //end of namespace 'engine'
