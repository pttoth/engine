/** -----------------------------------------------------------------------------
  * FILE:    SystemControl.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "pt/macros.h"
#include <string>

namespace engine{

PT_FORWARD_DECLARE_CLASS( SystemControl )

class SystemControl
{
public:
    virtual ~SystemControl(){}

    virtual std::string GetAssimpInfo() const = 0;
    virtual std::string GetLibPNGInfo() const = 0;
    virtual std::string GetCPUInfo() const = 0;
    virtual std::string GetGPUInfo() const = 0;
    virtual std::string GetGraphicsAPIInfo() const = 0;
    virtual std::string GetZLibInfo() const = 0;

    virtual std::string GetStrVRAMTotal() const = 0;
    virtual std::string GetStrVRAMAvailable() const = 0;
    virtual std::string GetStrVRAMEngineUsage() const = 0;

    virtual int GetVRAMTotal() const = 0;       // returns dedicated VRAM as kbytes
    virtual int GetVRAMAvailable() const = 0;   // returns dedicated VRAM as kbytes
    virtual int GetVRAMEngineUsage() const = 0; // returns dedicated VRAM as kbytes

protected:
private:

};

} //end of namespace 'engine'
