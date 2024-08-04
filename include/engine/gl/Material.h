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

    void Unbind();

    void LoadToVRAM();

    static MaterialPtr CreateFromFile( const std::string& name, const std::string& path );
    static MaterialPtr CreateFromString( const std::string& name, const std::string& data );

protected:
    enum Attribute{
        strTexture0Diffuse,
        strTexture0Normal,
        strTexture0Specular,
        strTexture1Diffuse,
        strTexture1Normal,
        strTexture1Specular,
        strVertexShader,
        strGeometryShader,
        strFragmentShader,
        strShaderProgramName
    };

    Material( const std::string& name );

    static MaterialPtr CreateFromString_ThrowsException( const std::string& name, const std::string& data ); // @TODO: refactor after 'ptlib' dropped exception-error logic
    static std::string GetConfigAttribute( const Material& mat, Attribute key );


    static void SetupConfigAttributes( pt::Config& cfg );

private:
    void Construct();

    bool                mDirty = true;
    bool                mInitialized = false;
    std::string         mName;
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
