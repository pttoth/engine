#pragma once

#include "pt/macros.h"
#include "pt/math.h"
#include <unordered_set>


//TODO: VecI2

namespace algo{

PT_FORWARD_DECLARE_STRUCT( Node )

struct Node
{
    std::unordered_set<int32_t> mEdges;
    int32_t id = -1;

    math::int2 pos;

    //-----
    Node( int32_t id_, const math::int2& pos_ )
        :id( id_ ), pos( pos_ )
    {}

    Node() = default;
    Node( const Node& rhs ) = default;
    Node( Node&& rhs ) = default;
    Node& operator=( const Node& rhs ) = default;
    Node& operator=( Node&& rhs ) = default;

};

} // end of namespace 'algo'
