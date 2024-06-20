#include "engine/actor/Light.h"

using namespace engine;

Light::
Light( const std::string& name ):
    Actor( name )
{}


Light::
~Light()
{}

void Light::SetLightColor(const math::vec4 &lightcolor)
{

}

math::vec4 Light::GetLightColor() const
{

}

void Light::LoadToVRAM()
{

}

void Light::OnTick(float t, float dt)
{

}

void Light::OnSpawned()
{

}

void Light::OnDespawned()
{

}

bool Light::OnCreateRenderContext()
{

}

void Light::OnDestroyRenderContext()
{

}

