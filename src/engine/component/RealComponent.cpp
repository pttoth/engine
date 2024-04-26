#include "engine/component/RealComponent.h"

#include "engine/Def.h"
#include "engine/service/DrawingControl.h"
#include "engine/Services.h"
#include "pt/logging.h"
#include "pt/utility.hpp"
#include <assert.h>

using namespace engine;

RealComponent::
RealComponent( const std::string& name , bool drawEnabled ):
    WorldComponent( name ), mDrawingEnabled( drawEnabled )
{}


RealComponent::
RealComponent( RealComponent&& source ):
    WorldComponent( std::move( source ) )
{
    PT_UNIMPLEMENTED_FUNCTION
}


RealComponent::
~RealComponent()
{
    //DestroyContext(); //here is not good, because derived class stack is destroyed by now
    //  mandating derivate classes to each clean up their render context which is not the virtual DestroyContext() function is asking for trouble
    //
    //  TODO: idea: make a resource monitor/manager service
    //      CreateContext registers the component and a stack of lambdas (containing the DestroyContext() codes) as an entry
    //      '~RealComponent()' will trigger these lambdas in reverse order and the resource manager will destroy the resources
    //      DestroyContext() can trigger this manually
    //  food for thought: how confusing is it, that the 'DestroyContext()' features will have to be written in a way
    //      taking into account that the class will already be destroyed when it runs?
}

RealComponent &RealComponent::
operator=( RealComponent&& source )
{
    PT_UNIMPLEMENTED_FUNCTION
}


void RealComponent::
Spawn()
{
    if( !IsSpawned() ){
        if( !mContextInitialized ){
            PT_LOG_WARN( "Late-init of render context for Component '" << GetName() << "'" );
            CreateContext();
        }
        WorldComponent::Spawn();    // calls 'OnSpawned()'

        auto dc = Services::GetDrawingControl();
        dc->AddDrawable( this );
    }
}


void RealComponent::
Despawn()
{
    if( IsSpawned() ){
        auto dc = Services::GetDrawingControl();
        dc->RemoveDrawable( this );

        WorldComponent::Despawn();
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


bool RealComponent::
IsRenderContextInitialized() const
{
    return mContextInitialized;
}


gl::DrawStage RealComponent::
GetPreferredDrawStage() const
{
    return gl::DrawStage::STANDARD;
}
