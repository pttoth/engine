#pragma once

#include "engine/experimental/SDLComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class SDLBillboardComponent;
using SDLBillboardComponentPtr  = std::shared_ptr< SDLBillboardComponent >;
using SDLBillboardComponentPtrW = std::weak_ptr< SDLBillboardComponent >;

class SDLBillboardComponent: public SDLComponent
{
public:
    SDLBillboardComponent( const std::string& name );
    SDLBillboardComponent( const SDLBillboardComponent& other );
    SDLBillboardComponent( SDLBillboardComponent&& source );

    virtual ~SDLBillboardComponent();

    SDLBillboardComponent& operator=( const SDLBillboardComponent& other );
    SDLBillboardComponent& operator=( SDLBillboardComponent&& source );

    bool operator==( const SDLBillboardComponent& other ) const;

    void Draw(float t, float dt) override;

protected:
    void OnSpawned() override;
    void OnDespawned() override;

    bool OnCreateContext() override;
    bool OnDestroyContext() override;
private:

};

} //end of namespace experimental
} //end of namespace engine
