#ifndef COORD_H
#define COORD_H

#include <math.h>

/// A Coordinate
/// with a row and a column.
struct Coord
{
  int row, col ;

  Coord() ;

  Coord( int irow, int icol ) ;

  Coord( const Coord& other ) ;

  // to shut the warnings up
  Coord( float irow, float icol ) ;

  void clampBetween( int minRow, int maxRow, int minCol, int maxCol ) ;

  // Finds manhattan distance to another Coord
  float manhattanTo( const Coord & other ) const ;

  // euclidean distance to another node.
  // we'll use this one for the heuristic function.
  float euclideanTo( const Coord & other ) const ;

  bool operator<( const Coord& other ) const ;

  bool operator==( const Coord& other ) const ;
} ;

#endif