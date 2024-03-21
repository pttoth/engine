#pragma once

#include "engine/Def.h"
#include "engine/actor/Actor.h"
#include "engine/actor/Camera.h"
#include "engine/component/WorldComponent.h"
#include "pt/macros.h"
#include "pt/math.h"

namespace engine{

math::float4x4 CalcMVP( const Actor& actor, const Camera& camera );
math::float4x4 CalcMVP( const WorldComponent& component, const Camera& camera );

math::float4x4 CalcRotMtx( const math::float3& direction, const math::float3& preferredUp );
math::float4x4 CalcScaleMtx( const math::float3& vec );
math::float4x4 CalcTranslationMtx( const math::float3& vec );


} // end of namespace 'engine'
