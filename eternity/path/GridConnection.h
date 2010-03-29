#ifndef GRIDCONNECTION_H
#define GRIDCONNECTION_H

enum GridConnection
{
  /// movement allowed in 4 directions
  /// (E, N, W, S) on grid
  FourWay,

  /// Movement allowed in 8 directions
  /// on grid (E, NE, N, NW, W, SW, S, SE)
  EightWay
} ;

#endif