#pragma once

#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Texture )

class Texture
{
public:
    Texture();
    Texture( const Texture& other );
    Texture( Texture&& source );

    virtual ~Texture();

    Texture& operator=( const Texture& other );
    Texture& operator=( Texture&& source );

    bool operator==( const Texture& other ) const;
protected:
private:
};

} //end of namespace engine
