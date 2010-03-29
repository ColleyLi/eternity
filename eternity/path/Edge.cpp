#include "Edge.h"

Edge::Edge()
{
  src=dst=0;
  cost = 0.0f;
}

Edge::Edge( GraphNode *sourceNode, GraphNode *destinationNode, float theCost )
{
  src = sourceNode ;
  dst = destinationNode ;
  cost = theCost ;
}