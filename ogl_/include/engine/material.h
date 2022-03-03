#pragma once

#include "engine/asset.h"
#include "engine/texture.h"

namespace pt{
namespace engine{

struct MaterialDescriptor: public AssetDescriptor{
    std::string texture_path;

    MaterialDescriptor();
    virtual ~MaterialDescriptor();
    MaterialDescriptor(const char* name_, const char* texture_path_);
    MaterialDescriptor(const std::string& name_, const std::string& texture_path_);
}; //end of struct MaterialDescriptor


class Material: public Asset{
    std::string mTextureName;
    Texture*    mTexture;

public:
    Material(const MaterialDescriptor& descriptor);
    virtual ~Material();

    inline const std::string& getTextureName() const{
        return mTextureName;
    }

    inline Texture* getTexture(){
        return mTexture;
    }

}; //end of class Material

} //end of namespace engine
} //end of namespace PT


