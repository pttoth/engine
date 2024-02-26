#include "engine/RealComponent.h"

#include "pt/logging.h"
#include "pt/utility.hpp"

#include <assert.h>

using namespace engine;

RealComponent::
RealComponent( const std::string& name ):
    WorldComponent( name )
{}


RealComponent::
~RealComponent()
{}


/*
void RealComponent::
Decouple()
{
    assert( false );
}
*/


const MeshPtr RealComponent::
GetMesh()
{
    return mMesh;
}


void RealComponent::
Spawn()
{
    if( !mContextInitialized ){
        CreateContext();
    }
    WorldComponent::OnSpawned();
}


void RealComponent::
Despawn()
{
    WorldComponent::Despawn();
    DestroyContext();
}


/*
void RealComponent::
OnSpawned()
{}


void RealComponent::
OnDespawned()
{}
*/


void RealComponent::
CreateContext()
{
    assert( !mContextInitialized );
    if( mContextInitialized ){
        pt::log::warn << "Trying to create context for an already initialized RealComponent: '" << this->GetName() << "'... skipping!\n";
        return;
    }

    bool success = OnCreateContext();
    assert( success );
    if( success ){
        mContextInitialized = true;
        PT_LOG_DEBUG( "Successfully created context for '" << this->GetName() );
    }else{
        pt::log::err << "Failed to create context for '" << this->GetName() << "'\n";
    }
}


void RealComponent::
DestroyContext()
{
    if( !mContextInitialized ){
        return;
    }

    bool success = OnDestroyContext();
    assert( success );
    if( success ){
        mContextInitialized = false;
        PT_LOG_DEBUG( "Successfully destroyed context for '" << this->GetName() );
    }else{
        pt::log::err << "Failed to destroy context for '" << this->GetName() << "'\n";
    }
}


void RealComponent::
Draw( float t, float dt )
{
#ifndef PT_DISABLE_DEBUG_OUTPUT
    if( !mContextInitialized ){
        pt::log::err << "Trying to draw element without an initialized render context. Skipping.\n";
        return;
    }
#endif
    OnDraw( t, dt );
}
