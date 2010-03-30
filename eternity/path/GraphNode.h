#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <set>
using namespace std ;

#include "Edge.h"
#include "Coord.h"

#define FLOAT_POSITIVE_INFINITY 1e22f
#define INTEGER_POSITIVE_INFINITY INT_MAX

class GraphNode ;
class Edge ;

typedef set<GraphNode *> /* as simply */ SetGraphNode ;
typedef set<GraphNode *>::iterator /* as simply */ SetGraphNodeIter ;

typedef deque<GraphNode *> /* as simply */ DequeGraphNode ;
typedef deque<GraphNode *>::iterator /* as simply */ DequeGraphNodeIter ;

class GraphNode
{
public:
  // Just data.  connectivity data
  // not stored here.

  // What WE COULD do is just store
  // a list<Edge *> edges as convenience
  vector<Edge *> edges ;

  // Have the node REMEMBER WHERE
  // in the map[][] it came from.
  // if we don't store this information
  // at graph construction time, then
  // the information about WHAT this
  // node is supposed to represent
  // is actually completely lost.
  Coord coord ;

  /// The coord links the GraphNode to the
  /// "reality" of the game world.
  /// Without this connection, the information
  /// about WHAT this Graph was constructed
  /// to represent in the GameWorld is LOST.
  /// If you were using a map with 2D points,
  /// this should be a Vector2.
  
  /// The TYPE of the additional information
  /// should really be a parameter as well,
  /// but I didn't want this to be unnecessarily
  /// confusing.
  //T additionalInformation ;

  // This is an indexed value again,
  // and it is set once a node has been
  // visited and a cost to arrive here
  // has been determined.
  // Because A* only "REACHES" each
  // node ONCE and ONLY ONCE,
  // this value gets set once and only
  // once when you first arrive at the node
  float costToGetHere ;


  GraphNode() ;
  ~GraphNode() ;

  // priority.
  bool operator<( const GraphNode& other ) ;

  // This is used when backtracing
  // the lowest cost path from the
  // endPos to the startPos.
  GraphNode* getLowestCostAdjacentNodeInSet( set<GraphNode*> list ) ;

} ;



#endif