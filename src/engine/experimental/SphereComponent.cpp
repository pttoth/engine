#include "engine/experimental/SphereComponent.h"

#include "engine/Services.h"
#include "engine/experimental/DrawingControl.h"

#include "pt/logging.h"

#include <assert.h>

using namespace engine;
using namespace engine::experimental;


SphereComponent::
SphereComponent( const std::string& name ):
    RealComponent( name )
{}


SphereComponent::
SphereComponent( const SphereComponent& other ):
    RealComponent( other )
{}


SphereComponent::
SphereComponent( SphereComponent&& source ):
    RealComponent( source )
{}


SphereComponent::
~SphereComponent()
{}


SphereComponent &SphereComponent::
operator=( const SphereComponent& other )
{}


SphereComponent &SphereComponent::
operator=( SphereComponent&& source )
{}


bool SphereComponent::
operator==( const SphereComponent& other ) const
{}


void SphereComponent::
Draw( float t, float dt )
{

}


void SphereComponent::
SetRadius( float r )
{
    if( r < 0.0f ){
        pt::log::err << "Tried to set negative radius (" << r << ") to SphereComponent '"
                     << this->GetName() << "'\n";
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

    auto dc = Services::GetDrawingControl2();
    bool suc = false;
    suc = dc->AddDrawable( this );
}


bool SphereComponent::
OnDestroyContext()
{
    auto dc = Services::GetDrawingControl2();


    return dc->RemoveDrawable( this );
}
