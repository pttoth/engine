#include "engine/component/RealComponent.h"

#include "engine/Def.h"
#include "engine/service/Renderer.h"
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
SetPreferredDrawStage( gl::RenderStage draw_stage )
{
    if( this->IsSpawned() ){
        PT_LOG_LIMITED_WARN( 50, "Tried setting new Draw Stage for already spawned component '" << GetName() << "'!" );
#ifdef PT_DEBUG_ENABLED
        static bool firsttime = true;
        if( firsttime ){
            pt::PrintStackTrace( "Setting new Draw Stage for spawned components is not yet supported!" );
            firsttime = false;
        }
#endif
    }
    mRenderStage = draw_stage;
}


void RealComponent::
Spawn()
{
    if( !IsSpawned() ){
        if( !mContextInitialized ){
            PT_LOG_WARN( "Late-init of render context for Component '" << GetName() << "'" );
            CreateRenderContext_GL3_3();
        }
        WorldComponent::Spawn();    // calls 'OnSpawned()'

        auto dc = Services::GetRenderer();
        dc->AddDrawable( this );
    }
}


void RealComponent::
Despawn()
{
    if( IsSpawned() ){
        auto dc = Services::GetRenderer();
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
CreateRenderContext_GL3_3()
{
    assert( !mContextInitialized );
    if( mContextInitialized ){
        PT_LOG_WARN( "Trying to create context for an already initialized RealComponent: '" << GetName() << "'... skipping!" );
        return;
    }

    bool success = OnCreateRenderContext_GL3_3();
    assert( success );
    if( success ){
        mContextInitialized = true;
        PT_LOG_DEBUG( "Successfully created context for '" << GetName() << "'" );
    }else{
        PT_LOG_ERR( "Failed to create context for '" << GetName() << "'" );
    }
}


void RealComponent::
CreateRenderContext_GL4_6()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void RealComponent::
DestroyRenderContext_GL3_3()
{
    if( !mContextInitialized ){
        return;
    }

    bool success = OnDestroyRenderContext_GL3_3();
    assert( success );
    if( success ){
        mContextInitialized = false;
        PT_LOG_DEBUG( "Successfully destroyed context for '" << GetName() );
    }else{
        PT_LOG_ERR(  "Failed to destroy context for '" << GetName() << "'" );
    }
}


void RealComponent::
DestroyRenderContext_GL4_6()
{
    PT_UNIMPLEMENTED_FUNCTION
}


void RealComponent::
Render_GL3_3( float t, float dt )
{
#ifdef ENGINE_DEBUG_ENABLED
    if( !mContextInitialized ){
        PT_LOG_ERR( "Trying to draw element without an initialized render context. Skipping." );
        return;
    }
#endif
    OnRender_GL3_3( t, dt );
}


void RealComponent::
Render_GL4_6( float t, float dt )
{
    PT_UNIMPLEMENTED_FUNCTION
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


gl::RenderStage RealComponent::
GetPreferredDrawStage() const
{
    return mRenderStage;
}
