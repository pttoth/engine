#pragma once

//#include "pt/name"

#include "engine/Texture.h"

#include <memory>
#include <string>

namespace engine{

class Material;
using MaterialPtr  = std::shared_ptr< Material >;
using MaterialPtrW = std::weak_ptr< Material >;

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
    TexturePtrW texture;
};

} //end of namespace engine
