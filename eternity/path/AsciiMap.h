#ifndef MAP_H
#define MAP_H

#include <list>
#include <vector>
#include <map>
#include <stack>
#include <queue>
#include <set>
using namespace std ;

#include "Coord.h"
#include "Graph.h"
#include "GraphNode.h"
#include "HighPerformanceTimer.h"
#include "GridConnection.h"
#include "AStar.h"

class Graph ;
class AStar ;

#define foreachRow for( int row = 0 ; row < mapRows ; row++ )
#define foreachCol for( int col = 0 ; col < mapCols ; col++ )

#define foreachTo(x,y) for( int x = 0 ; x < y ; x++ )

/// Set the "IMPASSIBLE" value
/// to a value no one would ever use
#define IMPASSIBLE -1e9

typedef map<char, float> /* as simply */ CostMap ;
typedef map<char, float>::iterator /* as simply */ CostMapIter ;

typedef deque<Coord> DequeCoord ;
typedef deque<Coord>::iterator DequeCoordIter ;


/// A AsciiMap is an abstraction for a 2D array of char,
/// the most important thing about this is it
/// remembers the Rows and Cols for the AsciiMap
class AsciiMap
{
private:
  char ** asciiMap ;
  char ** asciiMapSolved ;  // a copy of map, writes the
  // solution path into the map as 'X's,
  // used for debugging purposes.

  int mapRows, mapCols ;
  Coord startPos, endPos ;
  
  GridConnection gridConn ; // connectivity
  // style of the graph

  /// This is the relationship between
  /// a map tile and the "cost" to
  /// walk onto that tile.  You can
  /// really finely control/influence
  /// movement on the map this way.
  /// If a cost is the special value
  /// IMPASSIBLE, then it means
  /// that tile CANNOT BE WALKED UPON
  /// by the pathfinding algorithm.
  CostMap tileCosts ;

  /// The graph that represents this map
  /// If the map changes, then this Graph
  /// will have to be regenerated, but
  /// that is totally hidden from the user
  /// of this class.
  Graph *graph ;
  AStar *astarSolver ;

public:
  AsciiMap() ;

  /// Just creates an empty map
  /// with #rows, #cols you want
  AsciiMap( int rows, int cols ) ;

  /// Saves a local copy of the
  /// map, so you can delete yours
  /// if you use this.
  AsciiMap( char ** inputMap, int rows, int cols ) ;

  void initVars() ;

  ~AsciiMap() ;

  /// Gets you the internal ascii map.
  /// You will be able to edit the ascii map
  /// if you like, so when you call this
  /// the asciiMap is automatically set to "dirty"
  /// (i.e. when you call SOLVE the entire solution
  /// will be recomputed from scratch)
  char **getAsciiMap() ;

  /// Saves a local copy of the
  /// map, so you can delete yours
  /// if you use this.
  void setAsciiMap( char ** inputMap, int rows, int cols ) ;

  /// Gets you the solved ascii map.
  /// can edit this to your hearts delight..
  char **getAsciiMapSolved() ;

  /// No setter, rows is set
  /// with setSize()
  int getRows() ;

  /// No setter, cols is set
  /// with setSize()
  int getCols() ;

  /// Overload the [] operator
  /// to get map characters
  /// Too bad, this syntax returns
  /// you a 1D array really, then
  /// you can just index it again
  /// using result[col]
  char* operator[]( int row ) ;

  /// Overloads operator(), gives
  /// you a char& reference back
  /// so you can use it for assignment too.
  char& operator()( int row, int col ) ;

  /// Sets the size of the map
  /// internally
  void setSize( int rows, int cols ) ;

private:
  /// Copies a map to another
  /// Assumes both maps are mapRows x mapCols,
  /// so be careful.
  void copyMap( char **dst, char **src ) ;

  /// Deletes maps
  /// completely
  void deleteMaps() ;

public:
  /// Add a tile-type to the map,
  /// giving it a cost as well.
  /// Also use it in case you want to change
  /// the cost of a map character
  void setTileCost( char tile, float cost ) ;

  /// Gives you the cost to traverse
  /// a specific map character
  float getCostOf( char mapChar ) ;

  /// Cost of walking into tile
  /// at row, col in map
  float getCostAt( int row, int col ) ;

  /// Gets you the cost at a specific
  /// position in the map (involves lookup
  /// into getCostOf())
  float getCostAt( Coord pos ) ;

  /// Returns a COPY of the costMap to you,
  /// which isn't a very cheap operation,
  /// so you shouldn't be getting a costMap
  /// on every frame of your game loop
  CostMap getCostMapCopy() ;

  /// Gets you a random walkable tile
  /// from the underlying graph, really.
  /// Before calling this function you should
  /// have already set the MAP , costMap (setTileCost()) and
  /// also set connectivity (setGridConnection()),
  /// because they'll both influence
  /// what tiles are reachable.
  Coord getRandomWalkableTile() ;

private:
  /// An internal function to retrieve
  /// the "Nth" item from the costmap
  char getCostMapCharAt( UINT index ) ;

  /// An internal function to pull
  /// a random character from the costmap
  char getRandom() ;

public:
  /// Fill the map with random characters
  /// you've already added to your costmap
  /// @param setNumber The "setNumber is just
  /// the value to seed the random number generator
  /// with before pulling random values.
  void fillRandom( int setNumber ) ;

public:
  /// Constructs a graph out of your AsciiMap
  /// object.
  void constructGraph() ;

  void setStartPos( const Coord &start ) ;
  Coord getStartPos() ;
  void setEndPos( const Coord &end ) ;
  Coord getEndPos() ;

  GridConnection getGridConnection() ;
  void setGridConnection( GridConnection gridConnection ) ;

  DequeCoord solve() ;
  DequeCoord solve( const Coord & start, const Coord & end ) ;

  double getLastSolutionTime() ;
} ;


#endif