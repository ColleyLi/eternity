#include "AStar.h"
#include "../helperFunctions.h"

#if _DEBUG
#pragma message( __FILE__ ": PATHFINDING ONLY WORKS FAST IN RELEASE MODE!" )
#endif

AStar::AStar()
{
  initVars();
}

AStar::AStar( const Graph *theGraph )
{
  graph = theGraph ;
  initVars();
}

void AStar::initVars()
{
  finalPath = new DequeGraphNode ;

#if _DEBUG
  warning( "Pathfinder:  WARNING!!!  PATH-FINDING RUNNING IN DEBUG MODE."
    "THIS WILL NOT BE FAST!  TO GET BEST PERFORMANCE "
    "YOU SHOULD COMPILE AND RUN IN __RELEASE__ MODE!!" ) ;
#endif
}

AStar::~AStar()
{
  DESTROY( finalPath ) ; // delete the object,
  // but no necessarily the Graph * objects
  // that were passed in, because someone else
  // may still be using them.

  // DO NOT DELETE THE 'graph*' object!
  // It belongs to someone else.
}

void AStar::setGraph( const Graph *theGraph )
{
  graph = theGraph ;
}

// PULL means get it out and remove it 
// from the list and return it as well.
GraphNode* AStar::pullLowestCostNodeFromOpenList()
{
  // Like you shouldn't have pulled if
  // the list was empty
  if( openList.empty() )
  {
    error( "Pathfinder: The open list was EMPTY in pullLowestCostNode" ) ;
    return NULL ; // you get NULL.
  }

  float lowestCostSoFar = FLOAT_POSITIVE_INFINITY ;
  SetGraphNodeIter lowestCostNodeIter = openList.begin() ;
  GraphNode *lowestCostNode = *openList.begin() ;

  foreach( SetGraphNodeIter, iter, openList )
  {
    GraphNode* node = *iter ;

    if( node->costToGetHere < lowestCostSoFar )
    {
      lowestCostSoFar = node->costToGetHere ;
      lowestCostNodeIter = iter ;
      lowestCostNode = *iter ;
    }
  }

#if VERBOSITY
  printf( "Exploring from node (%d,%d) with cost %.3f\n",
    lowestCostNode->coord.row,
    lowestCostNode->coord.col,
    lowestCostNode->costToGetHere ) ;
#endif

  openList.erase( lowestCostNodeIter ) ; // remove it from the LIST
  return lowestCostNode ;
}

// A* algorithm.
// pseudocode:
// while the open list is not empty
// {
//   current_node = node from open list ___with lowest cost___ FROM START NODE
//   if current_node equals goal_node then
//     END ;
//   else
//     move current node to the closed list
//     examine each node adjacent to the current node
//     foreach adjacent node
//       if it isn't in the open list and it isn't in the closed list (and it isn't an obstacle # - then it wouldn't be considered an adjacent node, ninny!)
//         move it to the open list and CALCULATE ITS COST
// }
DequeGraphNode* AStar::solve( GraphNode *startNode, GraphNode *endNode )
{
  // Start the algorithm by clearing out
  // the openList and closedList, and adding
  // the starting node to the openList.
  clearPath();

  if( !startNode || !endNode )
  {
    error( "Pathfinder:  You must set BOTH startNode and endNode." ) ;
    return finalPath ;  // (it will be empty, it was just cleared above)
  }
  else
  {
    openList.insert( startNode ) ;
  }

  timer.start() ;

  bool solved = false ;

  while( !solved )
  {
    if( openList.empty() ) 
    {
      timer.stop() ;

      warning( "Pathfinder:  Sorry, the path could not be reached.  You can try again "
        "with a different start or end node" ) ;
      
      info( "Pathfinder:  * * * FAILURE to find path occurred in %f seconds * * * \n", timer.total_time ) ;

      // break the while loop without solving
      // you will get back an EMPTY list
      // because finalPath will just be empty
      // when the function returns (scroll to
      // end of this function to see)
      break ; 
    }
    else while( !openList.empty() )
    {
      // start by taking the front node.

      // really this should be the
      // "node with lowest cost to reach"
      // from the open list.
      GraphNode * current_node = pullLowestCostNodeFromOpenList() ;
      
      if( current_node == endNode )
      {
        timer.stop();

        //info( "Pathfinder: Algorithm done" ) ;

        solved = true ;

        // Clear off the open list as we are done.
        openList.clear() ;

        //info( "Pathfinder:  * * * Took %f seconds * * * \n", timer.total_time ) ;
        if( timer.total_time > 0.0166666667 )
        {
          static bool warned = false ; // make sure this warning only
          // happens once

          if( !warned )
          {
            warned = true ;
#if _DEBUG
          warning( "Pathfinder:  WARNING!  PATHFINDING EXTREMELY SLOW, YOU SHOULD "
            "COMPILE IN __RELEASE__ MODE" ) ;
#else
          warning( "Pathfinder:  WARNING!  Pathfinding on a graph that size "
            "not possible to do EVERY FRAME in real-time using your "
            "current hardware. "
            "You can still find the path ONCE every few frames, "
            "but if you try to do it every frame of your game "
            "you will slow the game down." ) ;
#endif
          }
        }

        //info( "Writing out final path . . . " ) ;

        // Write the final path by backtracking
        // along from the endPos back to the startPos,
        // lowest cost route, please.
        GraphNode *cn = endNode ;
        finalPath->push_front( cn ) ; // the end node
        // is the last node of the solution
        // We get the solution by backtracking
        // end node to start node, so
        // we push front
        //
        // +->  end
        // | 
        // front
        //
        // so that we get, by the end
        //
        // +-> start  X  X  X  X  end
        // | 
        // front                  back
        while( cn != startNode )
        {
#if VERBOSITY
          printf( "Next node is (%d,%d) with cost %f\n", cn->coord.row, cn->coord.col, cn->costToGetHere ) ;
#endif
          // Walk towards the lowest cost neighbour,
          // pushing each node into the finalPath
          // as we go
          
          cn = cn->getLowestCostAdjacentNodeInSet( closedList ) ;
          finalPath->push_front( cn ) ;
        }

        // Print lowest cost path
#if VERBOSITY
        puts( "" ) ;
        printf( "Lowest cost path is %f in %d steps\n", endNode->costToGetHere, finalPath.size() ) ;
        for( int i = 0 ; i < finalPath.size() ; i++ )
        {
          printf( "%d - (%d, %d) $%f\n", i+1, finalPath[i]->coord.row, finalPath[i]->coord.col, finalPath[i]->costToGetHere ) ;
        }
#endif
      }
      else
      {
        // move the current_node to the closedList
        closedList.insert( current_node ) ;

        // now working from the current_node,
        // we need to examine ALL adjacent nodes
        // to this current_node.
        for( UINT i = 0 ; i < current_node->edges.size() ; i++ )
        {
          GraphNode *adjacentNode = current_node->edges[ i ]->dst ;
          
          // if (the adjacent node) isn't  in open or closed list,
          // move it to the open list and compute cost.

          // ((A pretty nice optimization was
          // how the guy stored IS_OPEN_LIST inside
          // the nodes (instead of having to search lists)))
          if( openList.find( adjacentNode ) == openList.end() &&
              closedList.find( adjacentNode ) == closedList.end() )
          {
            // then its not in the open or closed list
            // so move it to the open list and compute its cost
            // to get there.
            openList.insert( adjacentNode ) ;
            
            // Compute the value of the HEURISTIC function
            // (which is implemented in Graph.h
            // just as the euclidean distance to
            // the goal node)
            float heuristicFnValue = graph->h( current_node, endNode ) ;
            // The heuristic function helps to "steer" the
            // exploration pattern and drive it to
            // the goal node

            adjacentNode->costToGetHere = current_node->costToGetHere +
              current_node->edges[ i ]->cost + heuristicFnValue ;
          }
        }
      }
    }
  }

  return finalPath ;
}

void AStar::clearPath()
{
  openList.clear();
  closedList.clear();
  finalPath->clear();
}

double AStar::getSolutionTime()
{
  return timer.total_time ;
}

