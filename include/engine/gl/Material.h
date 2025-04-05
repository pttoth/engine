#pragma once

#include "engine/Def.h"
#include "engine/gl/Texture2d.h"
#include "engine/gl/Shader.h"
#include "engine/gl/ShaderProgram.h"
#include "pt/config.h"
#include "pt/macros.h"

// TODO: remove instance-based file loaders
//       replace with static generator functions (factory pattern)

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( Material )

// this is serialized, so each entry should have a fix value
//  and some in-between space for future entries doesn't hurt
enum class MaterialType{

    // these are placeholders for ideas for now...
    //      trim what is not needed
    // in case of hierarchical entries (wood / tree) a fallback logic would be good
    //      if a specific collision sound (tree + something) is missing,
    //      it could fall back to a more generic one (wood + something)
    BRICK           = 100,
    CONCRETE        = 200,
    FLESH           = 300,
    METAL           = 400,
    PAPER           = 500,
        CARDBOARD   = 501,
    PLASTIC         = 600,
    WOOD            = 700,
        TREE        = 701,
    //...
};


class Material
{
public:
    virtual ~Material();

    Material( const Material& other ) = delete;
    Material( Material&& source ) = delete;
    Material& operator=( const Material& other ) = delete;
    Material& operator=( Material&& source ) = delete;
    bool operator==( const Material& other ) const = delete;

    void Bind();
    const std::string& GetName() const;
    bool IsClientSideSynced() const;
    bool IsStub() const;

    void Unbind();

    void LoadToVRAM();

    static MaterialPtr CreateFromFile( const std::string& name, const std::string& path );
    static MaterialPtr CreateFromString( const std::string& name, const std::string& data );
    static MaterialPtr CreateStubMaterial( const std::string& name );

protected:
    enum Attribute{
        strTexture0Diffuse      = 0,
        strTexture0Normal       = 1,
        strTexture0Specular     = 2,
        strTexture1Diffuse      = 3,
        strTexture1Normal       = 4,
        strTexture1Specular     = 5,

        strShaderProgramName    = 10000,
    };

    Material( const std::string& name );

    static MaterialPtr CreateFromString_NoLog( const std::string& name, const std::string& data );
    static std::string GetConfigAttribute( const Material& mat, Attribute key );

    static void SetupConfigAttributes( pt::Config& cfg );

private:
    static uint32_t GetTextureIndex( uint32_t slot, TexComponent texcomponent );

    static void     SetTextureAtIndex( MaterialPtr mat, uint32_t idx, const std::string& name );

    bool            HasStubTextures() const;
    void            Construct();




    bool                mDirty          = true;
    bool                mIsStub         = false;
    std::string         mName;
    std::string         mPath;
    MaterialType        mType; //TODO: add default value
    pt::Config          mCfg;

    static const uint32_t stTextureSlotCount    = 2;
    static const uint32_t stTextureCount        = 3*stTextureSlotCount;
    // only holds 'nullptr', if it is unused (empty name in material)
    std::vector<gl::Texture2dPtr>   mTextures;
    gl::ShaderProgramPtr mShaderProgram;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
