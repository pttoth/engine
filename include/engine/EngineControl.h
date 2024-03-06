/** -----------------------------------------------------------------------------
  * FILE:    EngineControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

namespace engine{

class EngineControl
{
public:
    virtual bool DeveloperMode() const = 0;

protected:
private:
};

} //end of namespace 'engine'
