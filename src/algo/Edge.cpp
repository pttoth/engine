#include "algo/Edge.h"

#include <assert.h>


bool algo::Edge::
operator==( const algo::Edge& rhs )
{
    return id == rhs.id;
}


int32_t algo::Edge::
GetConnectedNode( int32_t n ) const
{
    if( n1 == n ){
        return n2;
    }else if( n2 == n ){
        return n1;
    }else{
        assert( false );
        return -1;
    }
}


int32_t algo::Edge::
GetConnectedNode( const algo::Node& n ) const
{
    return GetConnectedNode( n.id );
}
