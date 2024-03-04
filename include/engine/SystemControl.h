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
    virtual std::string GetAssimpInfo() const;
    virtual std::string GetLibPNGInfo() const;
    virtual std::string GetCPUInfo() const;
    virtual std::string GetGPUInfo() const;
    virtual std::string GetGraphicsAPIInfo() const;
    virtual std::string GetZLibInfo() const;

protected:
private:
};

} //end of namespace 'engine'
