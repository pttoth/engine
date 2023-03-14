#pragma once

#include <memory>

namespace engine{

class Texture;
using TexturePtr  = std::shared_ptr< Texture >;
using TexturePtrW = std::weak_ptr< Texture >;

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
