#pragma once

#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wdouble-promotion"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#include "pt/math.h"
#pragma GCC diagnostic warning "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic warning "-Wreturn-type"
#pragma GCC diagnostic warning "-Wtype-limits"
#pragma GCC diagnostic warning "-Wunused-parameter"
#pragma GCC diagnostic warning "-Wunused-variable"
#pragma GCC diagnostic warning "-Wunused-value"
#pragma GCC diagnostic warning "-Wsign-conversion"
#pragma GCC diagnostic warning "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic warning "-Wdouble-promotion"
#pragma GCC diagnostic warning "-Wunknown-pragmas"

namespace pt{
    inline math::float3 Vecf3FromVecf4(const math::float4& vec){
        return math::float3(vec.x /vec.w,
                            vec.y /vec.w,
                            vec.z /vec.w);
    }
}


