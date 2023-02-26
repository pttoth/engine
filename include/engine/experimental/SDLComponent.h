#pragma once

#include "engine/experimental/RealComponent.h"

#include <memory>

namespace engine{
namespace experimental{

class SDLComponent;
using SDLComponentPtr  = std::shared_ptr< SDLComponent >;
using SDLComponentPtrW = std::weak_ptr< SDLComponent >;

class SDLComponent: public RealComponent
{
public:
    SDLComponent( const std::string& name );
    SDLComponent( const SDLComponent& other ) = delete;
    SDLComponent( SDLComponent&& source ) = delete;

    virtual ~SDLComponent();

    SDLComponent& operator=( const SDLComponent& other ) = delete;
    SDLComponent& operator=( SDLComponent&& source ) = delete;

    bool operator==( const SDLComponent& other ) const = delete;


    void Draw(float t, float dt) override;
    void Tick(float t, float dt) override;

protected:
    //void OnSpawned() override;
    //void OnDespawned() override;

    //bool OnCreateContext() override;
    //bool OnDestroyContext() override;
private:

};

} //end of namespace experimental
} //end of namespace engine
