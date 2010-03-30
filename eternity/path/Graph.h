#ifndef GRAPH_H
#define GRAPH_H

#include "AsciiMap.h"
#include "GraphNode.h"
#include "Edge.h"
#include "GridConnection.h"

class AsciiMap ;

/// Graph (V,E):  a series of
/// vertices ('nodes'), and weighted edges that are
/// the connections between the nodes.
class Graph
{
  vector<GraphNode *> nodes ;
  vector<Edge *> edges ;

public:
  /// Constructs a graph
  /// either 4-way or 8-way.
  /// 8 way means diagonal movements
  /// in the map are ALLOWED.
  /// @param asciiMap An asciiMap to make a graph out of
  /// @param gridConnection Four-way or eight-way grid connection.
  Graph( AsciiMap *asciiMap, GridConnection gridConnection ) ;
  ~Graph() ;

private:
  /// Used for map parsing.
  /// For each map node that
  /// is not IMPASSIBLE, it
  /// creates a graph node.
  void addNodesToGraph( AsciiMap *asciiMap ) ;

  /// Connects the graph up
  /// using information from
  /// the original AsciiMap
  void connectGraph( AsciiMap *asciiMap, GridConnection gridConnection ) ;

public:
  /// For getting the START node or END node
  /// when all you know is the COORDINATE
  /// of the start/end position.
  /// This is THE CONNECTION from tile positions
  /// to GRAPHNODE* objects.  Otherwise you don't know
  /// how to get the right GraphNode* for your tile position.
  GraphNode* getGraphNodeByCoord( const Coord & searchCoord ) ;

  /// Gets you a random node
  /// from the graph
  GraphNode* getRandomNode() ;

  /// Finds the heuristic function value
  /// distance between two nodes
  float h( GraphNode *A, GraphNode *B ) const ;

private:
  // Check if this node should connect to
  // the node at 'coord', and if so,
  // create an appropriately costed
  // EDGE for the connection.
  void checkNode( AsciiMap * asciiMap, GraphNode *node, const Coord &coord ) ;

} ;

#endif