#include "engine/Utility.h"

#include "pt/logging.h"

#include <sstream>
#include <assert.h>

#ifdef PT_PLATFORM_WINDOWS
#include<windows.h>
#include<dbghelp.h>
#endif


using namespace math;

#ifdef PT_PLATFORM_WINDOWS
void engine::helper::
PrintStackTrace( const char* additional_message )
{
    // @NOTE: this doesn't work well with MinGW
    // @TODO: add MinGW-compatible solution (see addr2line)

    fprintf( stderr, "Error: %s:\n", additional_message );
    fprintf( stderr, "---------- Stack trace ----------\n" );

    SymInitialize( GetCurrentProcess(), NULL, TRUE );

    const uint16_t  stack_size = 255;
    void*           stack[ stack_size ];
    const uint32_t  symbol_name_length_max = 1024;
    const uint32_t  symbol_data_cap = sizeof(SYMBOL_INFO) + (sizeof(char) * symbol_name_length_max);
    char            symbol_data[symbol_data_cap];
    uint16_t        frames;
    HANDLE          process;
    SYMBOL_INFO*    symbol = (SYMBOL_INFO*) symbol_data;

    symbol->MaxNameLen = symbol_name_length_max - 1;
    symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

    process = GetCurrentProcess();
    frames  = CaptureStackBackTrace( 0, stack_size, stack, NULL );
    if( stack_size == frames ){
        fprintf( stderr, "WARNING: Stack trace (size %d) is possibly incomplete!\n", frames );
    }

    // Print stack trace
    for( unsigned short i=0; i<frames; ++i ){
        SymFromAddr( process, (DWORD64)(stack[i]), 0, symbol );
        fprintf( stderr, "%i: 0x%0llX - %s\n",
                frames - i - 1,
                symbol->Address,
                symbol->Name );
    }

    fprintf( stderr, "\n" );
}
#endif


math::float4x4 engine::
CalcMVP( const engine::Actor& actor, const engine::Camera& camera )
{
    return camera.GetProjMtx() * camera.GetViewMtx() * actor.GetWorldTransform();
}


math::float4x4 engine::
CalcMVP( const engine::WorldComponent& component, const engine::Camera& camera )
{
    return camera.GetProjMtx() * camera.GetViewMtx() * component.GetWorldTransform();
}


math::float4x4 engine::
CalcLookAtMtx( const math::float3& target, const math::float3& preferredUp )
{
    const vec3 dir   = target.normalize();
    const vec3 right = dir.cross( preferredUp ).normalize();
    const vec3 up    = right.cross( dir );

    mat4 lookAt = mat4::identity;
    // right-handed system
    //X: right  (screen horizontal)     (thumb)
    //Y: up     (screen vertical)       (pointing finger)
    //Z: -dir   (points towards viewer) (middle finger)
    lookAt.m[0][0] = right.v[0];    lookAt.m[0][1] = right.v[1];    lookAt.m[0][2] = right.v[2];
    lookAt.m[1][0] = up.v[0];       lookAt.m[1][1] = up.v[1];       lookAt.m[1][2] = up.v[2];
    lookAt.m[2][0] = -dir.v[0];     lookAt.m[2][1] = -dir.v[1];     lookAt.m[2][2] = -dir.v[2];
    lookAt.m[3][3] = 1.0f;

    return lookAt;
}


math::float4x4 engine::
CalcLookAtMtx( const math::float4& target, const math::float4& preferredUp )
{
    return CalcLookAtMtx( target.XYZ(), preferredUp.XYZ() );
}



math::float4x4 engine::
CalcScaleMtx(const math::float3& vec )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][0] = vec.v[0];
    mtx.m[1][1] = vec.v[1];
    mtx.m[2][2] = vec.v[2];
    return mtx;
}

math::float4x4 engine::
CalcTranslationMtx(const math::float3& vec )
{
    math::float4x4 mtx = math::float4x4::identity;
    mtx.m[0][3] = vec.v[0];
    mtx.m[1][3] = vec.v[1];
    mtx.m[2][3] = vec.v[2];
    return mtx;
}


std::string engine::
ResolveMediaFilePath( const std::string& path )
{
    PT_WARN_UNIMPLEMENTED_FUNCTION
    if( 0 == path.length() ){
        return std::string();
    }
    return std::string( "../../media/" ) + path;
}


float4x4 engine::
BuildTransformMtx( const math::float3& pos,
                   const math::FRotator& rotation,
                   const math::float3& scale )
{
    math::float4x4  mat_scale = CalcScaleMtx( scale );
    math::float4x4  mat_orient = rotation.GetTransform();
    math::float4x4  mat_trans = CalcTranslationMtx( pos );
    return mat_trans * mat_orient * mat_scale;
}


float4x4 engine::
BuildTransformMtx( const math::float3& pos,
                   const math::float4x4& rotation,
                   const math::float3& scale)
{
    math::float4x4  mat_scale = CalcScaleMtx( scale );
    math::float4x4  mat_trans = CalcTranslationMtx( pos );
    return mat_trans * rotation * mat_scale;
}
