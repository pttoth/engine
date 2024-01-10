#pragma once

#include "algo/Edge.h"
#include "algo/Node.h"

#include "pt/math.h"

#include <memory>
#include <vector>

namespace algo{

struct Graph;
using GraphPtr       = std::shared_ptr< Graph >;
using ConstGraphPtr  = std::shared_ptr< const Graph >;
using GraphWPtr      = std::weak_ptr< Graph >;
using ConstGraphWPtr = std::weak_ptr< const Graph >;
using GraphUPtr      = std::unique_ptr< Graph >;
using ConstGraphUPtr = std::unique_ptr< const Graph >;

struct Graph
{
    std::vector<Node> mNodes;
    std::vector<Edge> mEdges;

    Edge&       GetEdgeByID( int32_t eidx );
    const Edge& GetEdgeByID( int32_t eidx ) const;
    Node&       GetNodeByID( int32_t nidx );
    const Node& GetNodeByID( int32_t nidx ) const;

    Node*       GetNodeByPosition( const math::int2& pos );
    const Node* GetNodeByPosition( const math::int2& pos ) const;

    int32_t GenerateEdgeID();
    int32_t GenerateNodeID();

private:
    int32_t mNextNodeId = 0;
    int32_t mNextEdgeId = 0;
};

} // end of namespace 'algo'
