#pragma once

#include "engine/Def.h"
#include "engine/gl/Texture2d.h"
#include "engine/gl/Shader.h"
#include "engine/gl/ShaderProgram.h"
#include "pt/config.h"
#include "pt/macros.h"

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
    Material();
    virtual ~Material();
    Material( Material&& source );
    Material& operator=( Material&& source );

    Material( const Material& other ) = delete;
    Material& operator=( const Material& other ) = delete;
    bool operator==( const Material& other ) const = delete;

    void Bind();
    void Clear();
    bool IsClientSideSynced() const;
    void ReadFile( const std::string& path );
    void Unbind();


    void LoadToVRAM();
protected:

private:
    void Construct();

    bool                mDirty = true;
    bool                mInitialized = false;
    std::string         mPath;
    MaterialType        mType; //TODO: add default value
    pt::Config          mCfg;
    gl::Texture2dPtr    mTexture0Diffuse;
    gl::Texture2dPtr    mTexture0Normal;
    gl::Texture2dPtr    mTexture0Specular;
    gl::Texture2dPtr    mTexture1Diffuse;
    gl::Texture2dPtr    mTexture1Normal;
    gl::Texture2dPtr    mTexture1Specular;
    gl::ShaderPtr       mVertexShader;
    gl::ShaderPtr       mGeometryShader;
    gl::ShaderPtr       mFragmentShader;
    gl::ShaderProgramPtr mShaderProgram;

};

} // end of namespace 'gl'
} // end of namespace 'engine'
