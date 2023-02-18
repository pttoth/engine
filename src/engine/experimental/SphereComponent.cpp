#include "engine/experimental/SphereComponent.h"

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

void SphereComponent::Draw(float t, float dt)
{

}

bool SphereComponent::OnCreateContext()
{

}

bool SphereComponent::OnDestroyContext()
{

}
