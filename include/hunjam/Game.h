#pragma once

#include "engine/Engine.h"

#include "hunjam/StakeActor.h"
#include "hunjam/WorldAxisActor.h"

#include "pt/macros.h"

PT_FORWARD_DECLARE_CLASS( Game )

class Game: public engine::Engine
{
public:
    struct MeshEntry{
        std::string mName;
        engine::gl::Mesh::FormatHint mHint;
        MeshEntry( std::string name, engine::gl::Mesh::FormatHint hint = engine::gl::Mesh::FormatHint::MD5_IDTECH4 ):
            mName( name ), mHint( hint )
        {}
    };

    Game() = delete;
    Game( int const argc, char* argv[] );
    Game( const Game& other ) = delete;
    Game( Game&& source ) = delete;
    virtual ~Game();
    Game& operator=( const Game& other ) = delete;
    Game& operator=( Game&& source ) = delete;
    bool operator==( const Game& other ) const = delete;
protected:
    void OnStart() override;
    void OnExit() override;
    void UpdateGameState_PreActorTick(float t, float dt) override;
    void UpdateGameState_PostActorTick(float t, float dt) override;

    void OnEvent( SDL_Event* event ) override;
    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

    void EnableFreeLook( bool value );

private:
    std::vector<std::string> mSkyboxes;
    size_t  mCurrentSkyboxIndex = 1;
    bool    mSkyboxEnabled = true;

    std::vector<MeshEntry> mMeshes;
    size_t mCurrentMeshIndex = 0;

    std::vector<std::string> mMaterials;
    std::vector<std::string> mTextures;

    const float mDefaultFoV     = 75.0f;
          int   mFoVAdjustment  = 0;

    bool mShootKeyDown = false;

    std::string mMediaURL;
    std::string mMediaHint;

    enum GameCfg{
        strMediaURL,
        strMediaHint,
        bMoveableActor,
        bMoveableSpotlight,
        bCacoCloseup,
        bShadowMapTesting,
        bNormalVectorTesting,
        bCirclingLights,
        bPlasmaGunInHand,
        bPreloadAllAssets,
    };

    enum MediaManifestCfg{
        bHasRequiredMedia,
    };

    pt::Config                  mGameCfg;
    pt::Config                  mMediaManifest;
    bool                        mHasRequiredMedia = false;
    bool                        mPreloadAllAssets = true;
    bool                        mFreeLook = false;
    bool                        mSkyboxSelectionActive = false;

    WorldAxisActorPtr           mWorldAxis;
    StakeActorPtr               mStakeActor;

};
