#include "GraphNode.h"

GraphNode::GraphNode()
{
  costToGetHere = 0 ;
}

GraphNode::~GraphNode()
{
  // !! DO NOT DELETE THE EDGES.
  // They are simply mirrored copies
  // of the edges in the global list.
  // the main game object will take care
  // of the global list.
}

// priority.
bool GraphNode::operator<( const GraphNode& other )
{
  return costToGetHere < other.costToGetHere ;
}

// This is used when backtracing
// the lowest cost path from the
// endPos to the startPos.
GraphNode* GraphNode::getLowestCostAdjacentNodeInSet( SetGraphNode list )
{
  if( edges.empty() )
    puts( "ERROR!! getLowestCostAdjacent node has an empty list" ) ;

  float lowestCostSoFar = FLOAT_POSITIVE_INFINITY ;
  GraphNode* lowestCostNeighbour = (*edges.begin())->dst ;

  for( vector<Edge*>::iterator edgeIter = edges.begin() ;
       edgeIter != edges.end() ;
       ++edgeIter )
  {
    Edge* edge = *edgeIter ;
    
    // I am src, so other is dst.
    GraphNode *n = edge->dst ;

    // only valid if n is in the closedList
    if( list.find( n ) != list.end() &&
        n->costToGetHere < lowestCostSoFar )
    {
      lowestCostSoFar = edge->dst->costToGetHere;
      lowestCostNeighbour = edge->dst ;
    }
  }

  return lowestCostNeighbour ;
}


