/** -----------------------------------------------------------------------------
  * FILE:    SystemManager.h
  * AUTHOR:  ptoth
  * EMAIL:   peter.t.toth92@gmail.com
  * -----------------------------------------------------------------------------
  */
#pragma once

#include "engine/service/SystemControl.h"

#include "engine/Utility.h"

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
    std::string GetPlatformSpecificParameters() const override;
    std::string GetZLibInfo() const override;

    const Version& GetDXVersion() const override;
    const Version& GetGLVersion() const override;
    const Version& GetVKVersion() const override;

    std::string GetStrTotalCombinedVideoMemory() const override; // VRAM + aux video memory (RAM)
    std::string GetStrVRAMTotal() const override;
    std::string GetStrVRAMAvailable() const override;
    std::string GetStrVRAMEngineUsage() const override;

    size_t GetMaximumUniformBlockBindingPoints() const override;
    int GetMaximumUniformBlocksCombined() const override;
    int GetMaximumUniformBlocksCompute() const override;
    int GetMaximumUniformBlocksFragment() const override;
    int GetMaximumUniformBlocksGeometry() const override;
    int GetMaximumUniformBlocksTessControl() const override;
    int GetMaximumUniformBlocksTessEval() const override;
    int GetMaximumUniformBlocksVertex() const override;

    int GetVRAMTotal() const override;          // returns dedicated VRAM as kbytes
    int GetVRAMAvailable() const override;      // returns dedicated VRAM as kbytes
    int GetVRAMEngineUsage() const override;    // returns dedicated VRAM as kbytes

    void Initialize() override;

protected:
private:
    Version mDXVersion;
    Version mGLVersion;
    Version mVKVersion;

    bool  mInitialized                  = false;
    size_t mMaxUniformBlockBindingPoints = 0;
    int   mMaxUniformBlocksCombined     = 0;
    int   mMaxUniformBlocksCompute      = 0;
    int   mMaxUniformBlocksFragment     = 0;
    int   mMaxUniformBlocksGeometry     = 0;
    int   mMaxUniformBlocksTessControl  = 0;
    int   mMaxUniformBlocksTessEval     = 0;
    int   mMaxUniformBlocksVertex       = 0;


};

} //end of namespace 'engine'
