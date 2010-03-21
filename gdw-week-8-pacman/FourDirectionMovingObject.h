#ifndef FOUR_DIRECTION_MOVING_OBJECT_H
#define FOUR_DIRECTION_MOVING_OBJECT_H

#include "pacman-main.h"

#include "GameObject.h"
//#include "GameWorld.h"

/// This class defines an object
/// that is restricted to only move
/// in 4 directions.  When there is
/// direction change it "lines the character up"
/// properly
/// All of the guys in Pacman are FourDirectionMovingObjects
/// including player ghost and walking bonus items
/// so their movement is handled by __common code__.
class FourDirectionMovingObject : public GameObject
{
protected:
  /// Speed in
  /// "pixels per frame".
  float speed ; 

public:
  // We'll use a "FINITE STATE MACHINE"
  // (AAAAAAAHH!!!) to keep track of what
  // "state" or "mode of operation" that
  // Pacman is in, with respect to his MOTION.

  // The FSM is handy because now when he's in the
  // MovingLeft state we automatically know 2 things about him:
  //   1.  His velocity is in the direction (x=-1, y=0)
  //   2.  His animation sprite is facing left
  
  // Because Pacman can only move in 1 of 4
  // directions at a time, an FSM is really
  // suitable here
  enum MotionState
  {
    Stopped,
    MovingLeft,
    MovingRight,
    MovingUp,
    MovingDown
  } ;

  // A member for Pacman's movement state
  MotionState motionState ;

  FourDirectionMovingObject() ;

  /// Moves the object a little.
  virtual void step( float time ) override ;

  /// For this to work, the DEFAULT stock position
  /// is the sprite is FACING RIGHT.
  /// This function assumes that
  /// your sprite:
  ///   ->    Initially faces right
  ///   ^     Rotates 90 degrees CCW to "face up"
  ///   <-    Flip it horizontally to face left
  ///   v     Rotates 90 degrees CW to "face down"
  /// If your sprite DOES NOT follow these behaviors
  /// then you should override this function.
  /// (for example, Inky, Blinky, Pinky each use
  ///  a DIFFERENT sprite for up/down movement than
  ///  for left/right movement,
  ///  Also the items use the same sprite
  ///  regardless of direction of motion!)
  virtual void draw() override ;

} ;

#endif