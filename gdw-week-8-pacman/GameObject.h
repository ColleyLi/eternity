#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "pacman-main.h"

#include "Vector2.h"

// A GameObject is a movable,
// intersectable entity..
class GameObject
{
public:
  Vector2 pos ;  // These are made public
  Vector2 vel ;  // for convenience.
  // Another option is to write an
  // overrideable (virtual) function
  // setPosition() to protect this member
  // from direct assignment if you need
  // to control what happens when the
  // position of a GameObject is changed.

  // Pacman physics .. we only
  // have position and velocity. There is
  // no acceleration.  Just constant velocity.
  // When you hit a wall you're stopped.

  // The sprite used to draw this object
  int spriteId ;

  GameObject() ;

  /// Overrideable function that
  /// you can use to move an object
  /// according to game rules.
  /// There is __no default behavior__.
  virtual void step( float time ) ;

  /// Overrideable function that
  /// you can use to draw an object
  /// according to its current state.
  /// Default behavior is to draw it
  /// @ position with spriteId
  virtual void draw() ;

} ;

#endif