#pragma once

#include <memory>

namespace algo{

struct BFSNode;
using BFSNodePtr       = std::shared_ptr< BFSNode >;
using ConstBFSNodePtr  = std::shared_ptr< const BFSNode >;
using BFSNodeWPtr      = std::weak_ptr< BFSNode >;
using ConstBFSNodeWPtr = std::weak_ptr< const BFSNode >;
using BFSNodeUPtr      = std::unique_ptr< BFSNode >;
using ConstBFSNodeUPtr = std::unique_ptr< const BFSNode >;

// BFS wrapper for Graph Node
//  contains pathfinding info about the node
struct BFSNode
{
    int32_t     nodeID      = -1;
    int32_t     edgeIDprev  = -1;   // shortest known direction from source
    int32_t     cost        = 0;    // lowest cost from source

    BFSNode() = default;
    BFSNode( int32_t nid );
    BFSNode( int32_t nid, int32_t nid_prev, int32_t cost_ );
};

} // end of namespace 'algo'
