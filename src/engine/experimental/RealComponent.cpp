#include "engine/experimental/RealComponent.h"

#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;

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


const MeshPtr RealComponent::GetMesh()
{
    return mMesh;
}


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
#ifdef DEBUG
        pt::log::debug << "Successfully created context for '" << this->GetName() << "'\n";
#endif
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
#ifdef DEBUG
        pt::log::debug << "Successfully destroyed context for '" << this->GetName() << "'\n";
#endif
    }else{
        pt::log::err << "Failed to destroy context for '" << this->GetName() << "'\n";
    }
}


/*
void RealComponent::
OnSpawned()
{
    assert( false );
}


void RealComponent::
OnDespawned()
{
    assert( false );
}
*/
