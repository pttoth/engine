#pragma once

//#include "pt/name"

#include "engine/Texture.h"
#include "pt/macros.h"
#include <string>

namespace engine{

PT_FORWARD_DECLARE_CLASS( Material )

class Material
{
public:
    Material();
    Material( const Material& other );
    Material( Material&& source );

    virtual ~Material();

    Material& operator=( const Material& other );
    Material& operator=( Material&& source );

    bool operator==( const Material& other ) const;
protected:
private:
    TextureWPtr texture;
};

} //end of namespace engine
