#include "algo/BFSNode.h"


algo::BFSNode::
BFSNode( int32_t nid ):
    nodeID( nid )
{}


algo::BFSNode::
BFSNode( int32_t nid, int32_t eid_prev, int32_t cost_ ):
        nodeID( nid ), edgeIDprev( eid_prev ), cost( cost_ )
{}
