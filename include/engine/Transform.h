#pragma once

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Transform )

class Transform
{
public:
    Transform();
    Transform( const Transform& other );
    Transform( Transform&& source );
    virtual ~Transform();
    Transform& operator=( const Transform& other );
    Transform& operator=( Transform&& source );

    bool operator==( const Transform& other ) const;
protected:
private:
};

} // end of namespace 'engine'
