#pragma once

#include <algo/Node.h>

#include "pt/macros.h"

namespace algo{

PT_FORWARD_DECLARE_STRUCT( Edge )

struct Edge
{
    int32_t id = -1;
    int32_t n1 = -1;
    int32_t n2 = -1;
    int32_t cost = 0;

    Edge() = default;
    Edge( int32_t id_, int32_t n1_, int32_t n2_, int32_t cost_):
        id( id_ ), n1( n1_ ), n2( n2_ ), cost( cost_ )
    {}

    Edge( const Edge& rhs ) = default;
    Edge( Edge&& rhs ) = default;
    Edge& operator=( const Edge& rhs ) = default;
    Edge& operator=( Edge&& rhs ) = default;


    bool operator==( const Edge& rhs );

    int32_t GetConnectedNode( int32_t id ) const;
    int32_t GetConnectedNode( const Node& n ) const;
};

} // end of namespace 'algo'
