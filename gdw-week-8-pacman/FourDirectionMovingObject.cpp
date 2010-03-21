#include "FourDirectionMovingObject.h"

FourDirectionMovingObject::FourDirectionMovingObject()
{
  motionState = MotionState::MovingRight ;
}

void FourDirectionMovingObject::step( float time ) // override
{
  // "Stepping" for any FourDirectionMovingObject is
  // moving in the direction of
  // his velocity

  // His direction of motion really
  // differs depending on his MOTIONSTATE.

  // Adjust his velocity depending on current
  // motion state.  here we'll just set the
  // DIRECTION of the velocity vector,
  switch( motionState )
  {
  case MotionState::MovingLeft:
    vel = Vector2( -1, 0 ) ;  // unit vector pointing left
    break ;

  case MotionState::MovingRight:
    vel = Vector2( 1, 0 ) ;   // unit vector pointing right
    break ;

  case MotionState::MovingUp:
    vel = Vector2( 0, -1 ) ;  // unit vector pointing up
    // remember that y=-1 is "UPWARDS"!!
    break ;

  case MotionState::MovingDown:
    vel = Vector2( 0, 1 ) ;   // unit vector pointing DOWN
    // +y is downwards
    break ;

  case MotionState::Stopped:
  default:
    vel = Vector2( 0, 0 ) ; // no vel

    // We can also stop his sprite here
    // from animating, if we want
    window->getSprite( spriteId )->setTimePerFrame( SPRITE_INFINITY_LONG ) ;
    break ;
  }

  // Change mag of vel to being
  // equal to speed (since vel
  // is a unit vector)
  vel *= speed ; // adjust length of vector to match exact speed

  // Ok, BEFORE MOVING HIM,
  // we have to make sure that
  // this MOTION won't run him
  // into a wall.

  Vector2 newProposedPos = pos + vel*time ;

  // CHECK IF THE newProposedPos is
  // INSIDE A WALL.  if it is, then
  // pacman can't make that move,
  // and we'll actually just leave him
  // where he is.
  
  //!!leftoff
  //char c = game->level[0][0] ;

  pos += vel ;   // move him 

  // Now, we have to detect if Pacman
  // collided with a wall.  If he did,
  // push him back off the wall.

}


void FourDirectionMovingObject::draw() // override
{
  switch( motionState )
  {
  case MotionState::MovingRight:
    // The sprite is assumed to be facing right
    //window->drawSprite( spriteId, 
    window->drawSprite( spriteId, pos.x, pos.y, 16, 16 ) ;
    break ;

  case MotionState::MovingLeft:
    // Flip left.  Not a rotation of 180 degrees
    // because that would make pacman upside down.
    window->drawSprite( spriteId, pos.x, pos.y, -16, 16 ) ;
    break ;

  case MotionState::MovingUp:
    // Rotate 90 degrees CCW
    window->drawSprite( spriteId, pos.x, pos.y,
      16, 16, -90 ) ;
    break ;

  case MotionState::MovingDown:
    // rotate 90 degrees CW
    window->drawSprite( spriteId, pos.x, pos.y,
      16, 16, 90 ) ;
    break ;
  }
}



