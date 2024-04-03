#pragma once

#include "engine/Def.h"
#include "pt/config.h"
#include "pt/macros.h"

#include "assimp/postprocess.h"
#include <unordered_map>

namespace engine{

PT_FORWARD_DECLARE_CLASS( AssimpConfig )

class AssimpConfig: public pt::Config
{

public:
    AssimpConfig();
    virtual ~AssimpConfig();

    AssimpConfig( const AssimpConfig& other ) = delete;
    AssimpConfig( AssimpConfig&& source ) = delete;
    AssimpConfig& operator=( const AssimpConfig& other ) = delete;
    AssimpConfig& operator=( AssimpConfig&& source ) = delete;
    bool operator==( const AssimpConfig& other ) const = delete;

    unsigned int GetParameterMask() const;

    bool GetAttrib( aiPostProcessSteps key ) const;

protected:

private:
    void InitializePostProcessAttribute( aiPostProcessSteps key );
    std::unordered_map<aiPostProcessSteps, bool> mPostProcessSteps;
    //TODO: move AssimpConfig out here, so it doesn't reallocate on every model load

};

} // end of namespace 'engine'
