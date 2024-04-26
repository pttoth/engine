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

protected:
private:

};

} //end of namespace 'engine'
