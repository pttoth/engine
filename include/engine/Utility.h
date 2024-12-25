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


struct Version{
    math::vec4 val;

    Version(){}
    Version( float v1, float v2, float v3, float v4 ):
        val( v1, v2, v3, v4 ){
    }

    Version( const Version& rhs ){
        val = rhs.val;
    }

    bool operator<=( const Version& rhs )
    {
        if( val.v[0] < rhs.val.v[0] ){
            return true;
        }else if( val.v[0] == rhs.val.v[0] ){

            if( val.v[1] < rhs.val.v[1] ){
                return true;
            }else if( val.v[1] == rhs.val.v[1] ){

                if( val.v[2] < rhs.val.v[2] ){
                    return true;
                }else if( val.v[2] == rhs.val.v[2] ){

                    if( val.v[3] < rhs.val.v[3] ){
                        return true;
                    }else if( val.v[3] == rhs.val.v[3] ){
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool operator==( const Version& rhs )
    {
        return (val == rhs.val).all();
    }

};

math::float4x4 CalcMVP( const Actor& actor, const Camera& camera );
math::float4x4 CalcMVP( const WorldComponent& component, const Camera& camera );

math::float4x4 CalcLookAtMtx( const math::float3& target, const math::float3& preferredUp );
math::float4x4 CalcLookAtMtx( const math::float4& target, const math::float4& preferredUp );

math::float4x4 CalcScaleMtx( const math::float3& vec );
math::float4x4 CalcTranslationMtx( const math::float3& vec );

math::float4x4 BuildTransformMtx( const math::float3& pos, const math::FRotator& rotation, const math::float3& scale );
math::float4x4 BuildTransformMtx( const math::float3& pos, const math::float4x4& rotation, const math::float3& scale );

std::string     ResolveMediaFilePath( const std::string& path );

} // end of namespace 'engine'

// @TODO: Windows version doesn't use 'pt::PrintStackTrace'
//          as I wanted to avoid an extra '-lDbghelp' linkage in 'ptlib'
//          that is needed in the current implementation of the function's win-version here
//        I'll need to look around, whether 'ptlib' can have a stack trace printer function without extra linking
//        This macro will probably be moved up to 'ptlib' at some point...

// engine project's stack trace printer
#ifdef PT_PLATFORM_LINUX
    #define __PT_FUNC_STACKTRACE_PRINTER(expr) pt::PrintStackTrace( expr )
#elif defined PT_PLATFORM_WINDOWS
    #define __PT_FUNC_STACKTRACE_PRINTER(expr) engine::helper::PrintStackTrace( expr )
#elif defined PT_PLATFORM_MAC
    //#define __PT_FUNC_STACKTRACE_PRINTER(expr) (__PT_VOID_CAST (0))
    #define __PT_FUNC_STACKTRACE_PRINTER(expr) PT_UNIMPLEMENTED_FUNCTION
#endif

//-----
#define PT_PRINT_STACKTRACE(expr) __PT_FUNC_STACKTRACE_PRINTER(expr)

#define PT_PRINT_STACKTRACE_LIMITED(log_limit, expr) \
    { \
        static size_t count = 1; \
        size_t limit = log_limit; \
        if( count <= limit ){ \
            __PT_FUNC_STACKTRACE_PRINTER( expr ); \
            ++count; \
        } \
    }

//-----
#ifdef PT_DEBUG_ENABLED
    #define PT_PRINT_DEBUG_STACKTRACE(expr) PT_PRINT_STACKTRACE( expr )
    #define PT_PRINT_DEBUG_STACKTRACE_LIMITED(log_limit, expr) PT_PRINT_STACKTRACE_LIMITED( log_limit, expr )
#else
    #define PT_PRINT_DEBUG_STACKTRACE(expr) (__PT_VOID_CAST (0))
    #define PT_PRINT_DEBUG_STACKTRACE_LIMITED(log_limit, expr) (__PT_VOID_CAST (0))
#endif

