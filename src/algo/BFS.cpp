#include "algo/BFS.h"

#include "pt/math.h"

#include <assert.h>


algo::BFS::
BFS( const algo::Graph& graph, int32_t src_id, int32_t target_id ):
    mGraph( graph ), mSourceID( src_id ), mTargetID( target_id )
{
    // Graph has index-bound checking, it can be skipped here
    mNodes.reserve( graph.mNodes.size() );
    for( const Node& n : graph.mNodes ){
        mNodes.push_back( BFSNode( n.id ) );
    }
}

algo::BFSNode& algo::BFS::
GetBFSNodeByID( int32_t nidx )
{
    if( (nidx < 0) || mNodes.size() <= (size_t) nidx ){
        throw std::invalid_argument( "invalid index" );
    }
    return mNodes[nidx];
}


const algo::BFSNode& algo::BFS::
GetBFSNodeByID( int32_t nidx ) const
{
    if( (nidx < 0) || mNodes.size() <= (size_t) nidx ){
        throw std::invalid_argument( "invalid index" );
    }
    return mNodes[nidx];
}


void algo::BFS::
Evaluate()
{
    if( not mIsEvaluated ){
        std::unordered_set<int32_t> current_nodes;
        std::unordered_set<int32_t> visited_nodes;

        current_nodes.insert( mSourceID );

        while( 0 < current_nodes.size() ){
            EvaluateNodes( current_nodes, visited_nodes );
        }
        mIsEvaluated = true;

        // if TargetPos was encountered, path exists
        mPathExists = ( 0 < visited_nodes.count( mTargetID ) );
        // if all Nodes were encountered, graph is connected
        mGraphIsConnected = ( visited_nodes.size() == mGraph.mNodes.size() );
    }
}


std::vector<int32_t> algo::BFS::
GetPath() const
{
    std::vector<int32_t> retval;
    // skip signalling missing evaluation check, user is always right :)
    if( mIsEvaluated && mPathExists ){
        const BFSNode* sourceNode = &( GetBFSNodeByID( mSourceID ) );
        const BFSNode* targetNode = &( GetBFSNodeByID( mTargetID ) );
        const BFSNode* currentNode = targetNode;

        size_t failsafe = 0;
        while( currentNode != sourceNode ){
            assert( failsafe <= mNodes.size() ); // failsafe to avoid infinite cycles
            ++failsafe;

            retval.push_back( currentNode->nodeID );

            const Edge& e = mGraph.GetEdgeByID( currentNode->edgeIDprev );
            currentNode = &( GetBFSNodeByID( e.GetConnectedNode( currentNode->nodeID ) ) );
        }

        retval.push_back( sourceNode->nodeID );

        // reverse retval vector order
        size_t count = retval.size();
        for( size_t i=0; i<count/2; ++i ){
            std::swap( retval[i], retval[(count-1)-i] );
        }
    }

    return retval;
}


bool algo::BFS::
IsContinuous() const
{
    // skip signalling missing evaluation check, user is always right :)
    return mGraphIsConnected;
}


bool algo::BFS::
PathExists() const
{
    // skip signalling missing evaluation check, user is always right :)
    return mPathExists;
}


void algo::BFS::
EvaluateNodes( std::unordered_set<int32_t>& currentNodes,
               std::unordered_set<int32_t>& visitedNodes )
{
    std::unordered_set<int32_t> newNodes;
    auto& g = mGraph;

    for( int32_t ni : currentNodes ){  // for each current leaf node
        BFSNode& bn = GetBFSNodeByID( ni );

        auto n = g.GetNodeByID( bn.nodeID );
        for( auto eidx : n.mEdges ){    // for each of the leaves' edges
            const Edge& e = g.GetEdgeByID( eidx );
            BFSNode& bn2 = GetBFSNodeByID( e.GetConnectedNode( n.id ) );

            int32_t currentCost = bn.cost + e.cost;
            // if connected node is newly encountered
            if( ( 0 == visitedNodes.count( bn2.nodeID ) )
                && ( 0 == currentNodes.count( bn2.nodeID ) )
                && ( 0 == newNodes.count( bn2.nodeID ) ) )
            {
                bn2.cost = currentCost;
                bn2.edgeIDprev = e.id;
                newNodes.insert( bn2.nodeID ); // register node as a new one
            }else{ // if connected node is already known
                // check and fix up the previous path to 'bn2' knowing the new cost
                BFSNode* optiCurrentNode = &bn;
                BFSNode* optiNextNode = &bn2;
                const Edge* optiEdge = &e; // edge ID between the two nodes

                // while using this new path is shorter than the existing ones to 'bn2'
                while( optiCurrentNode->cost + optiEdge->cost < optiNextNode->cost ){
                    // update new costs along the previously existing path to 'bn2'
                    const Edge* tempEdge = optiEdge;
                    BFSNode* tempNewNode = optiNextNode;
                    size_t tempCurrentNodeCost = optiCurrentNode->cost;

                    // step one back along 'bn2'-s earlier path
                    optiEdge = &( g.GetEdgeByID( optiNextNode->edgeIDprev ) );
                    optiNextNode = &( GetBFSNodeByID( optiEdge->GetConnectedNode( optiNextNode->nodeID ) ) );
                    optiCurrentNode = tempNewNode;

                    // set optimized direction and cost for node
                    optiCurrentNode->edgeIDprev = tempEdge->id;
                    optiCurrentNode->cost = tempCurrentNodeCost + tempEdge->cost;
                }
            }
        } // finished checking node's edges
        visitedNodes.insert( bn.nodeID );
    } // finished checking nodes

    // remove any currentNode from newNodes (might have been added multiple times)
    for( int32_t bni : currentNodes ){
        newNodes.erase( bni );
        visitedNodes.insert( bni );
    }
    // empty out 'currentNodes'
    currentNodes.erase( currentNodes.begin(), currentNodes.end() );
    assert( 0 == currentNodes.size() );
    for( int32_t bni : newNodes ){
        currentNodes.insert( bni );
    }
}
