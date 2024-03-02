#include "engine/SphereComponent.h"

#include "engine/Services.h"
#include "engine/DrawingControl.h"

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

    auto dc = Services::GetDrawingControl();
    bool suc = false;
    suc = dc->AddDrawable( this );

    return suc;
}


bool SphereComponent::
OnDestroyContext()
{
    auto dc = Services::GetDrawingControl();


    return dc->RemoveDrawable( this );
}
