#include "engine/experimental/SphereComponent.h"

#include "engine/Services.h"
#include "engine/service/DrawingControl.h"
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
Draw( float t, float dt )
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
OnCreateContext()
{
    //TODO: add Sphere info to the GPU

    auto dc = Services::GetDrawingControl();
    bool suc = false;
    //suc = dc->AddDrawable( this ); // shouldn't enable drawing here

    return suc;
}


bool SphereComponent::
OnDestroyContext()
{
    auto dc = Services::GetDrawingControl();


    return dc->RemoveDrawable( this );
}
