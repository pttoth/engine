#pragma once

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{
namespace gl{

PT_FORWARD_DECLARE_CLASS( UniformBlockLightingInfo )

class UniformBlockLightingInfo
{
public:
    UniformBlockLightingInfo();
    UniformBlockLightingInfo( const std::string& name );

    UniformBlockLightingInfo( const UniformBlockLightingInfo& other );
    UniformBlockLightingInfo( UniformBlockLightingInfo&& source );
    virtual ~UniformBlockLightingInfo();
    UniformBlockLightingInfo& operator=( const UniformBlockLightingInfo& other );
    UniformBlockLightingInfo& operator=( UniformBlockLightingInfo&& source );

    bool operator==( const UniformBlockLightingInfo& other ) const;

protected:

private:

};

} // end of namespace 'gl'
} // end of namespace 'engine'
