#include "engine/material.h"

using namespace pt;
using namespace pt::engine;

//-----------------------------
//      MaterialDescriptor
//-----------------------------

MaterialDescriptor::
MaterialDescriptor(){
}

MaterialDescriptor::
~MaterialDescriptor(){
}

MaterialDescriptor::
MaterialDescriptor(const char *name_, const char *texture_path_)
        :AssetDescriptor (name_), texture_path(texture_path_){
}

MaterialDescriptor::
MaterialDescriptor(const std::string &name_, const std::string &texture_path_)
        :AssetDescriptor (name_), texture_path(texture_path_){
}

//-----------------------------
//          Material
//-----------------------------

Material::
Material(const MaterialDescriptor &descriptor)
        :Asset (descriptor),
        mTextureName(descriptor.texture_path){
}

Material::
~Material(){
}




























