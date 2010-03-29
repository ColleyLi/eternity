#ifndef ASTAR_H
#define ASTAR_H

#include <set>
using namespace std ;

#include "GraphNode.h"
#include "Graph.h"
#include "HighPerformanceTimer.h"

class Graph ;

/// Usually written "A*"
///
/// The A* algorithm is a best-first search
/// algorithm.
/// A* finds the shortest path on a graph
/// from a start node to an end node.
/// A* guarantees a shortest path
/// on a graph if such a path exists.
class AStar
{

  /// The Graph we're doing pathfinding on.
  /// NOT FREED by this class, hence const.
  const Graph *graph ;

  SetGraphNode openList ;
  SetGraphNode closedList ;

  /// The final path (series of node hops)
  /// that the A* algorithm found when it ran
  /// This gets returned to the caller of "solve",
  /// so its a pointer to avoid copy construction.
  DequeGraphNode *finalPath ;

  /// A timer used to
  /// measure the performance
  /// of the algorithm
  /// You are warned if the pathfinding
  /// you are doing isn't going to happen in real-time
  /// (assumed 60fps)
  StopWatch timer ;

public:
  AStar() ;

  /// Construct an A* pathfinder object
  /// and automatically find the path
  /// on this new graph
  AStar( const Graph *theGraph ) ;

  ~AStar() ;

  void initVars() ;

  void setGraph( const Graph *theGraph ) ;

  // PULL means get it out and remove it 
  // from the list and return it as well.
  //!! this function should be removed if
  // we used a priority queue
  GraphNode* pullLowestCostNodeFromOpenList() ;

  /// Runs the pathfinding algorithm and
  /// gives you a deque of graphnodes to follow
  /// as the solution.  The reason a deque is
  /// used is so you can work from either the
  /// first node of the solution path, or
  /// the last node in the solution path.
  DequeGraphNode* solve( GraphNode *startNode, GraphNode *endNode ) ;

  void clearPath() ;

  double getSolutionTime() ;

} ;

#endif // ASTAR_H