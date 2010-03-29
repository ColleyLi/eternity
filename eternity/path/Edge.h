#ifndef EDGE_H
#define EDGE_H

#include "GraphNode.h"

class GraphNode ;

class Edge
{
public:
  GraphNode *src, *dst ;
  float cost ;

  Edge();
  Edge( GraphNode *sourceNode, GraphNode *destinationNode, float theCost );
} ;


#endif