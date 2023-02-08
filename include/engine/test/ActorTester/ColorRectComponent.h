#pragma once

#include "engine/experimental/SDLBillboardComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class ColorRectComponent;
using ColorRectComponentPtr  = std::shared_ptr< ColorRectComponent >;
using ColorRectComponentPtrW = std::weak_ptr< ColorRectComponent >;

class ColorRectComponent: public SDLBillboardComponent
{
public:
    ColorRectComponent( const std::string& name );
    ColorRectComponent( const ColorRectComponent& other );
    ColorRectComponent( ColorRectComponent&& source );

    virtual ~ColorRectComponent();

    ColorRectComponent& operator=( const ColorRectComponent& other );
    ColorRectComponent& operator=( ColorRectComponent&& source );

    bool operator==( const ColorRectComponent& other ) const;

    void Draw(float t, float dt);

protected:
    void OnSpawned();
    void OnDespawned();

    bool OnCreateContext();
    bool OnDestroyContext();

private:

};

} //end of namespace experimental
} //end of namespace engine
