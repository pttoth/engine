#pragma once



#include "engine/actor/Actor.h"

#include "engine/Def.h"
#include "pt/macros.h"

namespace engine{

PT_FORWARD_DECLARE_CLASS( Food )


class Food: public Actor
{
public:
    Food( const std::string& name );
    virtual ~Food();

    Food() = delete;
    Food( const Food& other ) = delete;
    Food( Food&& source ) = delete;
    Food& operator=( const Food& other ) = delete;
    Food& operator=( Food&& source ) = delete;
    bool operator==( const Food& other ) const = delete;

protected:
    void OnTick( float t, float dt ) override;
    void OnSpawned() override;
    void OnDespawned() override;
    bool OnCreateRenderContext() override;
    void OnDestroyRenderContext() override;

private:

};

} // end of namespace 'engine'
