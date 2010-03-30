#include "Graph.h"
#include "../helperFunctions.h"

Graph::Graph( AsciiMap *asciiMap, GridConnection gridConnection )
{
  addNodesToGraph( asciiMap ) ;
  connectGraph( asciiMap, gridConnection ) ;
}

Graph::~Graph()
{
  // destroy the old graph nodes AND edges.
  for( UINT i = 0 ; i < nodes.size() ; i++ )
    delete nodes[ i ] ;
  nodes.clear();

  for( UINT i = 0 ; i < edges.size() ; i++ )
    delete edges[ i ] ;
  edges.clear();

  // cached set
  //!!lookupGraphNodeByCoordinate.clear();
}

void Graph::addNodesToGraph( AsciiMap *asciiMap )
{
  // First, let's add ALL nodes into the
  // graph
  foreachTo(row,asciiMap->getRows())
  {
    foreachTo(col,asciiMap->getCols())
    {
      if( asciiMap->getCostAt( row, col ) != IMPASSIBLE ) 
      {
        // there's a node here.
        GraphNode *node = new GraphNode() ;
        
        node->coord.row = row ; // remember
        node->coord.col = col ; // where you're from.

        nodes.push_back( node ) ;

        // Now record ALSO in the graphNodeByCoordinateTable
        // table the GraphNode reference
        //!!setGraphNodeByCoord( node->coord, node ) ;
        
      }
    }
  }
}

void Graph::connectGraph( AsciiMap *asciiMap, GridConnection gridConnection )
{
  for( UINT i = 0 ; i < nodes.size() ; i++ )
  {
    GraphNode *node = nodes[ i ] ;

    int &row = node->coord.row ;
    int &col = node->coord.col ;

    // at each asciiMap element,
    int rowUp = row - 1 ;
    int rowDown = row + 1 ;
    int colLeft = col - 1 ;
    int colRight = col + 1 ;

    // CHECK what nodes
    // this node can reach
    // and at what cost.

    
    // Always do
    // check cardinal directions East, North, West, South
    
    // check E
    checkNode( asciiMap, node, Coord( row, colRight ) ) ;
    
    // check N
    checkNode( asciiMap, node, Coord( rowUp, col ) ) ;

    // check W
    checkNode( asciiMap, node, Coord( row, colLeft ) ) ;

    // check S
    checkNode( asciiMap, node, Coord( rowDown, col ) ) ;


    // now if using 8-connected grid,
    // 4 more connections to establish
    if( gridConnection == GridConnection::EightWay )
    {
      // check remainder of NE, NW, SW, SE
      // to make total 8 directions 

      // check NE
      checkNode( asciiMap, node, Coord( rowUp, colRight ) ) ;
      
      // check NW
      checkNode( asciiMap, node, Coord( rowUp, colLeft ) ) ;

      // check SW
      checkNode( asciiMap, node, Coord( rowDown, colLeft ) ) ;
      
      // check SE
      checkNode( asciiMap, node, Coord( rowDown, colRight ) ) ;
    }
  }
}

GraphNode* Graph::getRandomNode()
{
  if( nodes.size() == 0 )
  {
    warning( "Graph::getRandomNode:  graph is empty, you get NULL" ) ;
    return NULL ;
  }

  int pos = rand() % nodes.size() ;
  return nodes[ pos ] ;
}

/// For getting the START node or END node
/// when all you know is the COORDINATE
/// of the start/end position.
/// This is THE CONNECTION from tile positions
/// to GRAPHNODE* objects.  Otherwise you don't know
/// how to get the right GraphNode* for your tile position.
GraphNode* Graph::getGraphNodeByCoord( const Coord & searchCoord )
{
  if( nodes.empty() )
  {
    puts("!! Warning, there are no nodes in the graph" ) ;
    return NULL ;
  }

  // So, a graphnode knows its Coord,
  // but that's the only thing that knows it.
  // !!caching this would be good for performance
  foreach( vector<GraphNode*>::iterator, iter, nodes )
  {
    if( (*iter)->coord == searchCoord )
      return *iter ;
  }

  // Not found
  return NULL ;
}

float Graph::h( GraphNode *A, GraphNode *B ) const
{
  return A->coord.euclideanTo( B->coord ) ;
  //return A->coord.manhattanTo( B->coord ) ;
}

// Check if this node should connect to
// the node at 'coord', and if so,
// create an appropriately costed
// EDGE for the connection.
void Graph::checkNode( AsciiMap * asciiMap,
                       GraphNode *node,
                       const Coord &coord )
{
  // To check this coordinate, both the row and col
  // must be IN BOUNDS.
  if( ! ( coord.row >= 0 && coord.row < asciiMap->getRows() &&
      coord.col >= 0 && coord.col < asciiMap->getCols() ) )
  {
    // out of bounds.
    return ;
  }

  
  float cost = asciiMap->getCostAt( coord ) ;
  if( cost < 0 )
  {
    // Its INFINITE, no edge to <<dir>> then.
  }
  else
  {
    // Get the NODE at that place in the asciiMap.
    GraphNode *dst = getGraphNodeByCoord( coord ) ;
    
    if( dst )
    {
      // Create and save the edge
      // in the "global collection"
      Edge *edge = new Edge( node, dst, cost ) ;
      
      edges.push_back( edge ) ;

      // Now, COPY the edge into the node's list INDEX
      // too for faster lookup.
      node->edges.push_back( edge ) ;
    }
  }
}

