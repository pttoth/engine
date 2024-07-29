#pragma once

#include "engine/Def.h"
#include "engine/actor/Actor.h"
#include "engine/actor/Camera.h"
#include "engine/component/WorldComponent.h"
#include "pt/macros.h"
#include "pt/math.h"
#include "pt/utility.hpp"

namespace engine{

#ifdef PT_PLATFORM_WINDOWS
namespace helper{

void PrintStackTrace( const char* additional_message = nullptr );

} // end of namespace 'helper'
#endif

math::float4x4 CalcMVP( const Actor& actor, const Camera& camera );
math::float4x4 CalcMVP( const WorldComponent& component, const Camera& camera );

math::float4x4 CalcLookAtMtx( const math::float3& target, const math::float3& preferredUp );
math::float4x4 CalcLookAtMtx( const math::float4& target, const math::float4& preferredUp );

math::float4x4 CalcScaleMtx( const math::float3& vec );
math::float4x4 CalcTranslationMtx( const math::float3& vec );

std::string     ResolveMediaFilePath( const std::string& path );

// @TODO: Windows version doesn't use 'pt::PrintStackTrace'
//          as I wanted to avoid an extra '-lDbghelp' linkage in 'ptlib'
//          that is needed in the current implementation of the function's win-version here
//        I'll need to look around, whether 'ptlib' can have a stack trace printer function without extra linking
//        This macro will probably be moved up to 'ptlib' at some point...

// engine project's stack trace printer
#ifdef PT_PLATFORM_LINUX
    #define PT_PRINT_STACKTRACE(expr) pt::PrintStackTrace( expr )
#elif defined PT_PLATFORM_WINDOWS
    #define PT_PRINT_STACKTRACE(expr) engine::helper::PrintStackTrace( expr )
#elif defined PT_PLATFORM_MAC
    //#define PT_PRINT_STACKTRACE(expr) (__PT_VOID_CAST (0))
    #define PT_PRINT_STACKTRACE(expr) PT_UNIMPLEMENTED_FUNCTION
#endif
} // end of namespace 'engine'
