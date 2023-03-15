/** -----------------------------------------------------------------------------
  * FILE:    EngineControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include <cstdint>

namespace engine{

class EngineControl
{
public:
    virtual void     SetTickrate( uint32_t rate ) const = 0;
    virtual uint32_t GetTickrate() const = 0;

protected:
private:
};

} //end of namespace engine
