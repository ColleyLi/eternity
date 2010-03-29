#ifndef GHOST_H
#define GHOST_H

#include "pacman-main.h"

#include "FourDirectionMovingObject.h"

#include <queue>
using namespace std ;

class Ghost : public FourDirectionMovingObject
{
  // Retain a stack of "next moves" to use
  queue<MotionState> nextMoves ;

public:
  Ghost() ;
  ~Ghost() ;

  // (i)nky, (b)linky, (p)inky, (s)ue,
  // as named in lvl1.txt
  char name ;

  /// Ghost selects and enqueues a next move.
  /// Intelligence depending on which ghost
  /// he is.  Sue is supposed to be the meanest.
  void enqueueNextMove() ;

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