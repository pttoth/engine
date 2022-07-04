#include "engine/asset.h"

using namespace pt;
using namespace pt::engine;

//-----------------------------
//      AssetDescriptor
//-----------------------------

AssetDescriptor::
AssetDescriptor(){
}

AssetDescriptor::
~AssetDescriptor(){
}

AssetDescriptor::
AssetDescriptor(const char *name_): name(name_){
}

AssetDescriptor::
AssetDescriptor(const std::string &name_): name(name_){
}


//-----------------------------
//          Asset
//-----------------------------

Asset::
Asset()
        :mName(""){
}

Asset::
Asset(const AssetDescriptor &descriptor)
        :mName(descriptor.name){
}

Asset::
~Asset(){
}
