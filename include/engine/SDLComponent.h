#pragma once

#include "engine/RealComponent.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( SDLComponent )

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

protected:
    //void OnSpawned() override;
    //void OnDespawned() override;

    //void OnTick( float t, float dt ) override;

    //bool OnCreateContext() override;
    //bool OnDestroyContext() override;
private:

};

} //end of namespace 'engine'
