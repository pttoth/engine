#pragma once

#include <string>

namespace pt{
namespace engine{

struct AssetDescriptor{
    std::string name;

    AssetDescriptor();
    virtual ~AssetDescriptor();
    AssetDescriptor(const char* name_);
    AssetDescriptor(const std::string& name_);
}; //end of struct AssetDescriptor

class Asset{
    std::string mName;
public:
    Asset();
    Asset(const AssetDescriptor& descriptor);
    virtual ~Asset();
    inline const std::string& getName() const{
        return mName;
    }
}; //end of class Asset


} //end of namespace engine
} //end of namespace PT
