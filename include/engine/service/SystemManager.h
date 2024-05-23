/** -----------------------------------------------------------------------------
  * FILE:    SystemManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/service/SystemControl.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( SystemManager )

class SystemManager: public SystemControl
{
public:
    SystemManager();
    SystemManager( const SystemManager& other ) = delete;
    SystemManager( SystemManager&& other ) = delete;
    virtual ~SystemManager();
    SystemManager& operator=( const SystemManager& other )  = delete;
    SystemManager& operator=( SystemManager&& other )       = delete;
    bool operator==( const SystemManager& other ) const     = delete;

    std::string GetAssimpInfo() const override;
    std::string GetLibPNGInfo() const override;
    std::string GetCPUInfo() const override;
    std::string GetGPUInfo() const override;
    std::string GetGraphicsAPIInfo() const override;
    std::string GetZLibInfo() const override;

    std::string GetStrVRAMTotal() const override;
    std::string GetStrVRAMAvailable() const override;
    std::string GetStrVRAMEngineUsage() const override;

    int GetVRAMTotal() const override;          // returns dedicated VRAM as kbytes
    int GetVRAMAvailable() const override;      // returns dedicated VRAM as kbytes
    int GetVRAMEngineUsage() const override;    // returns dedicated VRAM as kbytes

protected:
private:

};

} //end of namespace 'engine'
