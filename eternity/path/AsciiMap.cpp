#include "AsciiMap.h"
#include "Graph.h"
#include "../helperFunctions.h"
#include "../perlin.h"

AsciiMap::AsciiMap()
{
  initVars();
}

AsciiMap::AsciiMap( int rows, int cols )
{
  initVars();
  setSize( rows, cols ) ;
}

AsciiMap::AsciiMap( char ** inputMap, int rows, int cols )
{
  // copy over the input asciiMap
  initVars() ;
  setAsciiMap( inputMap, rows, cols ) ;
}

void AsciiMap::initVars()
{
  asciiMap = asciiMapSolved = 0 ;
  graph = 0 ;
  
  gridConn = GridConnection::FourWay ; // start assuming 4-way connection
  // between tiles

  astarSolver = new AStar() ;
}

AsciiMap::~AsciiMap()
{
  deleteMaps();
}

char ** AsciiMap::getAsciiMap()
{
  DESTROY( graph ) ; // he's going to dirty
  // the map, so the graph gets destroyed
  // at this point

  return asciiMap ;
}

void AsciiMap::setAsciiMap( char ** inputMap, int rows, int cols )
{
  deleteMaps() ;  // must delete existing maps if they exist
  setSize( rows, cols ) ;
  copyMap( asciiMap, inputMap ) ;
  copyMap( asciiMapSolved, inputMap ) ;
}

char ** AsciiMap::getAsciiMapSolved()
{
  return asciiMapSolved ;
}

int AsciiMap::getRows()
{
  return mapRows ;
}

int AsciiMap::getCols()
{
  return mapCols ;
}

char* AsciiMap::operator[]( int row )
{
  return asciiMap[ row ] ;
}

char& AsciiMap::operator()( int row, int col )
{
  static char outOfBoundsChar = 'X' ;
  if( row < 0 || col < 0 || row >= mapRows || col >= mapCols )
  {
    error( "Pathfinder:  Error.  (%d, %d) Out of bounds", row, col ) ;
    return outOfBoundsChar ;
  }
  else
    return asciiMap[row][col];
}

void AsciiMap::setSize( int rows, int cols )
{
  mapRows = rows ;
  mapCols = cols ;

  // Destroy them if they exist
  deleteMaps() ;

  asciiMap = new char*[ mapRows ] ;
  asciiMapSolved = new char*[ mapRows ] ;
  foreachRow
  {
    asciiMap[ row ] = new char[ mapCols ] ;
    asciiMapSolved[ row ] = new char[ mapCols ] ;
  }
}

void AsciiMap::copyMap( char **dst, char **src )
{
  foreachRow
    foreachCol
      dst[row][col]=src[row][col];
}

void AsciiMap::deleteMaps()
{
  if( asciiMap )
  {
    foreachRow
      delete[] asciiMap[row] ;
    delete[] asciiMap ;
    asciiMap = 0 ;
  }

  if( asciiMapSolved )
  {
    foreachRow
      delete[] asciiMapSolved[row] ;
    delete[] asciiMapSolved ;
    asciiMapSolved = NULL ;
  }
}

void AsciiMap::setTileCost( char tile, float cost )
{
  if( tileCosts.find( tile ) != tileCosts.end() ) 
  {
    warning( "Pathfinder:  tile %c already existed, overwriting its cost, %f", tile, cost ) ;
    tileCosts[ tile ] = cost ;
  }
  else
  {
    tileCosts.insert( make_pair( tile, cost ) ) ;
  }

  // !! The asciiMap is "dirty" now.
  // The edge weights
  // in the graph need to be updated.
  
  // We'll DELETE the graph here, since
  // it no longer counts, and re-generate it
  // ONCE, when the user asks for a solution
  DESTROY( graph ) ;
}

float AsciiMap::getCostOf( char mapChar )
{
  CostMapIter loc = tileCosts.find( mapChar ) ;
  if( loc != tileCosts.end() )
  {
    // Then it was found
    return loc->second ;
  }
  else
  {
    // print a warning..
    warning( "Pathfinder:  %c not found in your cost map!"
      "  Adding it with cost 1.0", mapChar ) ;
    
    tileCosts[ mapChar ] = 1.0f ;

    // Now it exists for sure
    return tileCosts[ mapChar ] ;
  }
}

float AsciiMap::getCostAt( int row, int col )
{
  return getCostOf( asciiMap[ row ][ col ] ) ;
}

float AsciiMap::getCostAt( Coord pos )
{
  return getCostOf( asciiMap[ pos.row ][ pos.col ] ) ;
}

CostMap AsciiMap::getCostMapCopy()
{
  return tileCosts ;
}

Coord AsciiMap::getRandomWalkableTile()
{
  // Instead of hit/miss approach
  // where you keep looking up tiles
  // until you get one that hits,
  // just pull one from the graph,
  // because the graph will only
  // have nodes for tiles
  // that hit
  if( !graph )
  {
    constructGraph() ;
  }

  return graph->getRandomNode()->coord ;
}

char AsciiMap::getCostMapCharAt( UINT index )
{
  if( index >= tileCosts.size() )
  {
    warning("Pathfinder:  there aren't %d tiles in the CostMap, you get '.'", index);
    return '.' ;
  }

  CostMapIter iter = tileCosts.begin() ;
  advance( iter, index ) ; // move forward "index" times
  return iter->first ;
}

char AsciiMap::getRandom()
{
  if( tileCosts.empty() )
  {
    warning("Pathfinder:  tileCosts are empty.  Creating tile '.', cost=1.0..");
    setTileCost( '.', 1.0f ) ;
  }
  CostMapIter iter = tileCosts.begin() ;
  advance( iter, rand()%tileCosts.size() ) ;

  return iter->first ;
}

void AsciiMap::fillRandom( int setNumber )
{
  // This dirties the solution..
  DESTROY( graph ) ;

  // seed random number generator

  srand( setNumber ) ;  // USE MAPSET (whatever user wants)

  // Use "seeding the random number generator" to your advantage!!
  // rand() is pseudorandom, so
  // the sequence of numbers you get
  // starting from any given SEED
  // is COMPLETELY PRE-DETERMINED!
  // This means you'll get the SAME
  // MAP as long as you SEED the random
  // number generator with the SAME value!

  /*
  // chaotic
  foreachRow
    foreachCol
      asciiMap[row][col] = 
        getRandom() ;
  */

  
  // Smooth
  // must reboot perlin noise to get
  // different noise values for re-seeded
  // random number generator
  perlin_noise_reboot = true ;

  int numSymbols = tileCosts.size() ;
  
  foreachRow
  {
    foreachCol
    {
      // get a perlin noise value
      double x = (double)row/mapRows ;
      double y = (double)col/mapCols ;

      // Perlin noise is just "smooth noise"
      // Experiment with it!
      double noise = PerlinNoise2D( x, y, 2, 2, 8 ) ;

      // The noise function is between -1 and +1,
      // so to make it a positive value between [0,1], we'll just
      // add 1 and divide by 2
      noise = (noise+1.0) / 2.0 ;

      // Now depending on noise, choose a symbol
      // from the map.
      int symbolIndex = (int)(numSymbols * noise) ;

      asciiMap[row][col] = getCostMapCharAt( symbolIndex ) ;
    }
  }



}

/// Constructs a graph out of your AsciiMap
/// object.
void AsciiMap::constructGraph()
{
  DESTROY( graph ) ; // if it exists, destroy it
  graph = new Graph( this, gridConn ) ;
}

void AsciiMap::setStartPos( const Coord &start )
{
  startPos = start ;
}
Coord AsciiMap::getStartPos()
{
  return startPos ;
}
void AsciiMap::setEndPos( const Coord &end )
{
  endPos = end ;
}
Coord AsciiMap::getEndPos()
{
  return endPos ;
}

GridConnection AsciiMap::getGridConnection()
{
  return gridConn ;
}

void AsciiMap::setGridConnection( GridConnection gridConnection )
{
  gridConn = gridConnection ;
  // dirties the graph
  DESTROY( graph ) ;
}

DequeCoord AsciiMap::solve()
{
  DequeCoord coordSolution ;

  // re-copy the asciiMap to asciiMapSolved
  // in case its been solved before
  copyMap( asciiMapSolved, asciiMap ) ;

  // He may be calling SOLVE a second time
  // on the same map, without having changed
  // anything in the map geometry.  In this case
  // finding a solution is fast, because you
  // won't have to wait to construct the graph.
  if( !graph )
    constructGraph() ;
  
  astarSolver->setGraph( graph ) ;

  GraphNode* startNode = graph->getGraphNodeByCoord( startPos ) ;
  if( !startNode )
  {
    warning( "Pathfinder: startNode invalid: (%d, %d) is impassible, "
      "there is no solution.",
    startPos.row, startPos.col ) ;

    return coordSolution ; // empty
  }
  GraphNode* endNode = graph->getGraphNodeByCoord( endPos ) ;
  if( !endNode )
  {
    warning( "Pathfinder: endNode invalid: (%d, %d) is impassible, "
      "there is no solution.",
    endPos.row, endPos.col ) ;
    
    return coordSolution ; // empty 
  }

  DequeGraphNode *graphSolution = astarSolver->solve( startNode, endNode ) ;

  // relate the DequeGraphNode to a DequeCoord
  foreach( DequeGraphNodeIter, gn, (*graphSolution) )
  {
    coordSolution.push_back( (*gn)->coord ) ;

    // write the solution
    asciiMapSolved[ (*gn)->coord.row ][ (*gn)->coord.col ] = 'X' ;
  }

  // Write in the start and end pos
  asciiMapSolved[ startPos.row ][ startPos.col ] = 'S' ;
  asciiMapSolved[ endPos.row ][ endPos.col ] = 'G' ;

  return coordSolution ;
}

DequeCoord AsciiMap::solve( const Coord & start, const Coord & end )
{
  setStartPos( start ) ;
  setEndPos( end ) ;

  return solve() ;
}

double AsciiMap::getLastSolutionTime()
{
  return astarSolver->getSolutionTime() ;
}

