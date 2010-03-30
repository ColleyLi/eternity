#ifndef GHOST_H
#define GHOST_H

#include "pacman-main.h"

#include "FourDirectionMovingObject.h"

#include <queue>
#include <deque>
using namespace std ;

class Ghost : public FourDirectionMovingObject
{
  const static int FramesToWaitBetweenPathComputes = 250 ;
  /// (i)nky, (b)linky, (p)inky, (s)ue,
  /// as named in lvl1.txt
  char name ;

  /// Retain a deque of "next moves" to use.
  /// This is updated every second or so
  /// with a new series of paths to follow
  deque<Coord> nextMoves ;

  /// Keep track of the number of frames that passed
  /// since you last did a pathfind
  int framesSinceLastRecomputeMoves ;

  /// Where does this ghost
  /// want to go to now?
  Coord goalNode ;


public:
  Ghost() ;
  ~Ghost() ;

  /// Name has an effect on
  /// shadingColor and ghost
  /// movement behavior
  void setName( char newName ) ;

  void pathfindTowardsRandomPosition() ;

  /// When a Ghost gets its "step" event,
  /// I want to "update" the "reqDir" (next move direction).
  virtual void step( float time ) override ;

  /// Override the draw() function to draw
  /// ghosts properly.  Note the change
  /// from the original pacman:  the ghosts
  /// eyes point in the direction he will
  /// head in next.
  virtual void draw() override ;

  
} ;

#endif