#pragma once

#include "engine/Actor.h"
#include "engine/Camera.h"
#include "engine/Def.h"
#include "engine/WorldComponent.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{
/*
std::string ToString( const math::float3& vec );
std::string ToString( const math::float4x4& mat );
*/

math::float4x4 CalcMVP( const Actor& actor, const Camera& camera );
math::float4x4 CalcMVP( const WorldComponent& component, const Camera& camera );

math::float4x4 CalcRotMtx( const math::float3& direction, const math::float3& preferredUp );
math::float4x4 CalcScaleMtx( const math::float3& scale );
math::float4x4 CalcTranslationMtx( const math::float3& scale );


} // end of namespace 'engine'
