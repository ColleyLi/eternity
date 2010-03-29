#include "Coord.h"
#include "../helperFunctions.h"

Coord::Coord()
{
  row = col = 0 ;
}

Coord::Coord( int irow, int icol )
{
  row = irow ;
  col = icol ;
}

Coord::Coord( const Coord& other )
{
  row = other.row ;
  col = other.col ;
}

// to shut the warnings up
Coord::Coord( float irow, float icol )
{
  row = (int)irow ;
  col = (int)icol ;
}

void Coord::clampBetween( int minRow, int maxRow, int minCol, int maxCol )
{
  clamp( row, minRow, maxRow ) ;
  clamp( col, minCol, maxCol ) ;
}

// Finds manhattan distance to another Coord
float Coord::manhattanTo( const Coord & other ) const 
{
  float x = fabs( (float)(other.col - col) ) ;
  float y = fabs( (float)(other.row - row) ) ;

  return x + y ;
}

// euclidean distance to another node.
// we'll use this one for the heuristic function.
float Coord::euclideanTo( const Coord & other ) const
{
  float x = fabs( (float)(other.col - col) ) ;
  float y = fabs( (float)(other.row - row) ) ;

  return sqrt( x + y ) ;
}

bool Coord::operator<( const Coord& other ) const
{
  // we'll use the magnitude,
  // considering them like vectors.
  if( row < other.row )
    return true ;
  else if( row == other.row )
    return col < other.col ;
  return false ;
}

bool Coord::operator==( const Coord& other ) const
{
  return row == other.row && col == other.col ;
}
