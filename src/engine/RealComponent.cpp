#include "engine/RealComponent.h"

#include "engine/Def.h"
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


const MeshPtr RealComponent::
GetMesh()
{
    return mMesh;
}


void RealComponent::
Spawn()
{
    if( !IsSpawned() ){
        if( !mContextInitialized ){
            PT_LOG_WARN( "Late-init of render context for Component '" << GetName() << "'" );
            CreateContext();
        }
        WorldComponent::Spawn();
    }
}


void RealComponent::
Despawn()
{
    if( IsSpawned() ){
        WorldComponent::Despawn();
        DestroyContext();
    }
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
        PT_LOG_WARN( "Trying to create context for an already initialized RealComponent: '" << GetName() << "'... skipping!" );
        return;
    }

    bool success = OnCreateContext();
    assert( success );
    if( success ){
        mContextInitialized = true;
        PT_LOG_DEBUG( "Successfully created context for '" << GetName() << "'" );
    }else{
        PT_LOG_ERR( "Failed to create context for '" << GetName() << "'" );
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
        PT_LOG_DEBUG( "Successfully destroyed context for '" << GetName() );
    }else{
        PT_LOG_ERR(  "Failed to destroy context for '" << GetName() << "'" );
    }
}


void RealComponent::
Draw( float t, float dt )
{
#ifdef ENGINE_DEBUG_ENABLED
    if( !mContextInitialized ){
        PT_LOG_ERR( "Trying to draw element without an initialized render context. Skipping." );
        return;
    }
#endif
    OnDraw( t, dt );
}


void RealComponent::
EnableDraw( bool enabled )
{
    mDrawingEnabled = enabled;
}


bool RealComponent::
IsDrawEnabled() const
{
    return mDrawingEnabled;
}


gl::DrawStage RealComponent::
GetPreferredDrawStage() const
{
    return gl::DrawStage::STANDARD;
}
