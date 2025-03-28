#include "engine/experimental/SphereComponent.h"

#include "engine/Services.h"
#include "engine/service/Renderer.h"
#include "pt/logging.h"
#include <assert.h>

using namespace engine;


SphereComponent::
SphereComponent( const std::string& name ):
    RealComponent( name )
{}


SphereComponent::
~SphereComponent()
{}


void SphereComponent::
Render_GL3_3( float t, float dt )
{

}


void SphereComponent::
SetRadius( float r )
{
    if( r < 0.0f ){
        PT_LOG_ERR( "Tried to set negative radius (" << r << ") to SphereComponent '" << GetName() << "'" );
    }else{
        mRadius = r;
    }
}


float SphereComponent::
GetRadius() const
{
    return mRadius;
}


bool SphereComponent::
OnCreateRenderContext_GL3_3()
{
    //TODO: add Sphere info to the GPU

    auto dc = Services::GetRenderer();
    bool suc = false;
    //suc = dc->AddDrawable( this ); // shouldn't enable drawing here

    return suc;
}


bool SphereComponent::
OnDestroyRenderContext_GL3_3()
{
    auto dc = Services::GetRenderer();


    return dc->RemoveDrawable( this );
}
