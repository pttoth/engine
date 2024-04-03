#pragma once

#include "engine/Def.h"
#include "pt/config.h"
#include "pt/macros.h"

#include <unordered_map>

namespace engine{

PT_FORWARD_DECLARE_CLASS( MaterialConfig )

class MaterialConfig: public pt::Config
{
public:
    MaterialConfig();
    virtual ~MaterialConfig();

    MaterialConfig( const MaterialConfig& other ) = delete;
    MaterialConfig( MaterialConfig&& source ) = delete;
    MaterialConfig& operator=( const MaterialConfig& other ) = delete;
    MaterialConfig& operator=( MaterialConfig&& source ) = delete;
    bool operator==( const MaterialConfig& other ) const = delete;

    //std::string& GetAttrib( aiPostProcessSteps key ) const;

protected:
private:
};

} // end of namespace 'engine'
