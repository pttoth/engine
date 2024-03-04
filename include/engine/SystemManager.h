/** -----------------------------------------------------------------------------
  * FILE:    SystemManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/SystemControl.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( SystemManager )

class SystemManager: public SystemControl
{
public:
    std::string GetAssimpInfo() const override;
    std::string GetLibPNGInfo() const override;
    std::string GetCPUInfo() const override;
    std::string GetGPUInfo() const override;
    std::string GetGraphicsAPIInfo() const override;
    std::string GetZLibInfo() const override;

protected:
private:

};

} //end of namespace 'engine'
