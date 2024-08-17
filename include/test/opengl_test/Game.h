#pragma once

#include "engine/Engine.h"

#include "test/opengl_test/RotationTestActor.h"
#include "test/opengl_test/WorldGeometry.h"

#include "BillboardActor.h"
#include "WorldAxisActor.h"
#include "Pawn.h"
#include "engine/actor/LightCone.h"
#include "engine/actor/LightPoint.h"
#include "engine/component/AxisDisplayComponent.h"
#include "engine/service/SystemManager.h"
#include "engine/gl/Texture2d.h"
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

    void OnMouseButtonDown(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseButtonUp(int32_t x, int32_t y, uint8_t button, uint8_t clicks, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseMotion(int32_t x, int32_t y, int32_t x_rel, int32_t y_rel, uint32_t timestamp, uint32_t mouseid) override;
    void OnMouseWheel(int32_t x, int32_t y, uint32_t timestamp, uint32_t mouseid, uint32_t direction) override;
    void OnKeyDown(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;
    void OnKeyUp(SDL_Keycode keycode, uint16_t keymod, uint32_t timestamp, uint8_t repeat) override;

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

    bool mSkyboxSelectionActive = false;
    bool mMeshSelectionActive = false;
    bool mFovSelectionActive = false;
    bool mLightAngleSelectionActive = false;
    bool mNormalSetupActive = false;
    float mLightAngle = 60.0f;

    bool mFreeLook = false;
    bool mLMBDown = false;
    bool mForwardDown = false;
    bool mBackDown = false;
    bool mLeftDown = false;
    bool mRightDown = false;
    bool mAscendDown = false;
    bool mDescendDown = false;

    bool mShiftDown = false;

    bool mUpArrowDown = false;
    bool mDownArrowDown = false;
    bool mLeftArrowDown = false;
    bool mRightArrowDown = false;
    bool mPageUp_Down = false;
    bool mPageDown_Down = false;
    bool mHomeDown = false;
    bool mEndDown = false;

    bool mRotationMode = false;

    bool mRotXDown = false;
    bool mRotX_Down = false;
    bool mRotYDown = false;
    bool mRotY_Down = false;
    bool mRotZDown = false;
    bool mRotZ_Down = false;

    bool mMoveableActor = false;
    bool mMoveableSpotlight = false;
    bool mCacoCloseup = false;
    bool mShadowMapTesting = false;
    bool mCirclingLights = false;
    bool mNormalVectorTesting = false;


    enum GameCfg{
        bMoveableActor,
        bMoveableSpotlight,
        bCacoCloseup,
        bShadowMapTesting,
        bNormalVectorTesting,
        bCirclingLights,

    };

    pt::Config                  mGameCfg;

    engine::gl::Texture2dPtr    mBillboardTexture;
    BillboardActor              mBillboardActor;
    RotationTestActorPtr        mRotationTestActor;
    WorldAxisActorPtr           mWorldAxis;
    WorldGeometryPtr            mWorldGeometry;
    engine::LightConePtr        mLightConeActor;
    engine::LightPointPtr       mLightPointActor1;
    engine::LightPointPtr       mLightPointActor2;
    engine::LightPointPtr       mLightPointActor3;
    engine::LightPointPtr       mLightPointActor4;

    engine::LightConePtr        mShadowMapTestingSpotlight1;
    engine::LightConePtr        mCacoCloseUpSpotlight1;
    engine::LightConePtr        mFixedLightCone1;

};
