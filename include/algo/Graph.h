#pragma once

#include "algo/Edge.h"
#include "algo/Node.h"
#include "pt/macros.h"
#include "pt/math.h"
#include <vector>

namespace algo{

PT_FORWARD_DECLARE_STRUCT( Graph )

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
