#include "algo/Graph.h"

#include <assert.h>
#include <sstream>
#include <stdexcept>

algo::Edge& algo::Graph::
GetEdgeByID( int32_t eidx )
{
    assert( (0 <= eidx) && (eidx < mNextEdgeId) );
    if( (eidx < 0) || (mNextEdgeId <= eidx ) ){
        std::stringstream ss;
        ss << "Invalid Graph edge ID(" << eidx << ")";
        throw std::invalid_argument( ss.str() );
    }
    return mEdges[ eidx ];
}


const algo::Edge& algo::Graph::
GetEdgeByID( int32_t eidx ) const
{
    assert( (0 <= eidx) && (eidx < mNextEdgeId) );
    if( (eidx < 0) || (mNextEdgeId <= eidx ) ){
        std::stringstream ss;
        ss << "Invalid Graph edge ID(" << eidx << ")";
        throw std::invalid_argument( ss.str() );
    }
    return mEdges[ eidx ];
}


algo::Node& algo::Graph::
GetNodeByID( int32_t nidx )
{
    assert( (0 <= nidx) && (nidx < mNextNodeId) );
    if( (nidx < 0) || (mNextNodeId <= nidx ) ){
        std::stringstream ss;
        ss << "Invalid Graph node ID(" << nidx << ")";
        throw std::invalid_argument( ss.str() );
    }
    return mNodes[ nidx ];
}


const algo::Node& algo::Graph::
GetNodeByID( int32_t nidx ) const
{
    assert( (0 <= nidx) && (nidx < mNextNodeId) );
    if( (nidx < 0) || (mNextNodeId <= nidx ) ){
        std::stringstream ss;
        ss << "Invalid Graph node ID(" << nidx << ")";
        throw std::invalid_argument( ss.str() );
    }
    return mNodes[ nidx ];
}


algo::Node* algo::Graph::
GetNodeByPosition( const math::int2& pos )
{
    for( auto& n : mNodes ){
        if( (n.pos == pos).all() ){
            return &n;
        }
    }
    return nullptr;
}


const algo::Node* algo::Graph::
GetNodeByPosition( const math::int2& pos ) const
{
    for( auto& n : mNodes ){
        if( (n.pos == pos).all() ){
            return &n;
        }
    }
    return nullptr;
}


int32_t algo::Graph::
GenerateEdgeID()
{
    int32_t retval = mNextEdgeId;
    ++mNextEdgeId;
    return retval;
}


int32_t algo::Graph::
GenerateNodeID()
{
    int32_t retval = mNextNodeId;
    ++mNextNodeId;
    return retval;
}
