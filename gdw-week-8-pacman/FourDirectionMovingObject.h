#ifndef FOUR_DIRECTION_MOVING_OBJECT_H
#define FOUR_DIRECTION_MOVING_OBJECT_H

#include "pacman-main.h"

#include "GameObject.h"
#include "Tile.h"

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

  /// Shading color to use
  /// when drawing this FourDirectionMovingObject
  D3DCOLOR shadingColor ;

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

  // requested motion state
  MotionState requestedMotionState ;

  FourDirectionMovingObject() ;

  /// A static function because logically
  /// this function isn't tied to any instance
  /// variables.  Its a C-style function
  /// in the middle of the object, because
  /// it takes any old vector, determines the dominant
  /// direction (-x, +x, -y or +y) and returns
  /// the "MotionState" that approximates that
  static MotionState getNearestDirection( Vector2 & vec )
  {
    // Determine the largest magnitude
    // direction
    if( fabs( vec.x ) > fabs( vec.y ) )
    {
      // Magnitude's bigger in x,
      // dominant direction will be in x
      if( vec.x > 0 )
        return MotionState::MovingRight ;  // +x is ->
      else
        return MotionState::MovingLeft ;   // -x is <-
    }
    else
    {
      // magnitude's bigger in y,
      // dominant direction will be in y
      if( vec.y > 0 )
        return MotionState::MovingDown ;   // +y is v
      else
        return MotionState::MovingUp ;     // -y is ^^
    }
  }

  /// "Rounds off" object position
  /// so that it is NEVER "infringing"
  /// on an obstacle's block.
  void squareOff( float ts ) ;

  /// Returns TRUE when you are
  /// aligned to a square.  This is
  /// usually used to check to see
  /// if you are allowed to move in
  /// another direction (can only do
  /// so when you ARE square).
  bool isSquare( float tileSize ) ;

  /// Request a change in direction
  /// If possible to go that direction then
  /// this immediately does it, but if not
  /// possible then the request is queued
  /// and tried every "step()"
  /// This function returns true when the
  /// move is allowed, returns false when
  /// the move is disallowed
  bool reqMotionState( MotionState newMotionState ) ;

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
  
  /// What to do when this FourDirectionMovingObject
  /// is intersected by a tile
  virtual void doIntersect( Tile *tile ) override ;

  /// What to do when this FourDirectionMovingObject
  /// is intersected by another GameObject
  virtual void doIntersect( GameObject *other ) override ;

  /// What to do when this FourDirectionMovingObject
  /// is intersected by another FourDirectionMovingObject
  virtual void doIntersect( FourDirectionMovingObject *other ) ;

} ;

#endif