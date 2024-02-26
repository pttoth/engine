#pragma once

#include <algo/BFSNode.h>
#include <algo/Graph.h>
#include "pt/macros.h"
#include <unordered_set>
#include <vector>


namespace algo{

PT_FORWARD_DECLARE_CLASS( BFS )

// Breadth-First Search graph wrapper
class BFS
{
public:
    // public members
    const Graph& mGraph;
    const int32_t mSourceID = -1;
    const int32_t mTargetID = -1;

    // ctors
    BFS( const Graph& graph, int32_t src_id, int32_t target_id );

    // member functions
    BFSNode& GetBFSNodeByID( int32_t nidx );
    const BFSNode& GetBFSNodeByID( int32_t nidx ) const;

    void Evaluate(); // calculate path
    std::vector<int32_t> GetPath() const; // return path
    bool IsContinuous() const;
    bool PathExists() const;
private:
    void EvaluateNodes( std::unordered_set<int32_t>& currentNodes,
                        std::unordered_set<int32_t>& visitedNodes );

    bool mIsEvaluated       = false;
    bool mGraphIsConnected  = false;
    bool mPathExists        = false;
    std::vector<BFSNode> mNodes;
};

} // end of namespace 'algo'
