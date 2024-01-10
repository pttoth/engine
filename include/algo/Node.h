#pragma once

#include "pt/math.h"

#include <memory>
#include <unordered_set>


//TODO: VecI2

namespace algo{

struct Node;
using NodePtr       = std::shared_ptr< Node >;
using ConstNodePtr  = std::shared_ptr< const Node >;
using NodeWPtr      = std::weak_ptr< Node >;
using ConstNodeWPtr = std::weak_ptr< const Node >;
using NodeUPtr      = std::unique_ptr< Node >;
using ConstNodeUPtr = std::unique_ptr< const Node >;

struct Node
{
    std::unordered_set<int32_t> mEdges;
    int32_t id = -1;

    pt::math::int2 pos;

    //-----
    Node( int32_t id_, const pt::math::int2& pos_ )
        :id( id_ ), pos( pos_ )
    {}

    Node() = default;
    Node( const Node& rhs ) = default;
    Node( Node&& rhs ) = default;
    Node& operator=( const Node& rhs ) = default;
    Node& operator=( Node&& rhs ) = default;

};

} // end of namespace 'algo'
