#include "FourDirectionMovingObject.h"
#include "GameWorld.h"

FourDirectionMovingObject::FourDirectionMovingObject()
{
  motionState = MotionState::MovingRight ;
  speed = 0 ;
}

void FourDirectionMovingObject::squareOff()
{
  // Ok, we have this situation:

  /*
        0   1   2   3   4  
      +---+---+---+---+---+ 0   <- pixel
      |   |   |   |###|   |
    0 |   |   |   |###|   |
      |   |   |   |###|   |
      +---+---+---+---+---+ 16  <- pixel
      |   |   |P  |###|   |
    1 |   |   PPP |###|   |
      |   |   |P  |###|   |
      +---+---+---+---+---+ 32
      |   |   |   |   |   |
    2 |   |   |   |   |   |
      |   |   |   |   |   |
      +---+---+---+---+---+ 48
      |   |   |   |   |   |
    3 |   |   |   |   |   |
      |   |   |   |   |   |
      +---+---+---+---+---+ 64
pixel 0   16  32  48  64  80

  Here (P)acman is about to run
  into a ###WALL###.

  That's not good.

  Pacman is at tile row=1, col=2,
  pixel location about x=36, y=24.

  So that means Pacman should be
  allowed to move forward...

        0   1   2   3   4  
      +---+---+---+---+---+ 0   <- pixel
      |   |   |   |###|   |
    0 |   |   |   |###|   |
      |   |   |   |###|   |
      +---+---+---+---+---+ 16  <- pixel
      |   |   | P |###|   |
    1 |   |   |PPP|###|   |
      |   |   | P |###|   |
      +---+---+---+---+---+ 32
pixel 0   16  32  48  64  80

  Now he's at pixel x=40, y=24...

  The question is here:

  SHOULD HE BE ALLOWED TO MOVE
  RIGHT ANYMORE???????

        0   1   2   3   4  
      +---+---+---+---+---+ 0   <- pixel
      |   |   |   |###|   |
    0 |   |   |   |###|   |
      |   |   |   |###|   |
      +---+---+---+---+---+ 16  <- pixel
      |   |   |  P|###|   |
    1 |   |   | PPP###|   | PACMAN PARTLY IN WALL.. bad news.
      |   |   |  P|###|   |
      +---+---+---+---+---+ 32
pixel 0   16  32  48  64  80

  NO!!!  He will start to punch
  into the wall and it will look
  very silly.


  So we're going to solve this problem by,
  at every step, ROUNDING UP and ROUNDING DOWN
  Pacman's coordinates, finding if he
  impinges or encroaches on a wall a bit,
  if he does, PUSH HIM OFF THE DADGAM WALL.

  This will also give the game a very squarey
  control type feel.  For Pacman it is suitable
  because it is like the original, but for your
  tile-based game, you might not want this behavior
  exactly.

  At this point the math of it and the
  gameplay controls are closely intertwined..
  */

  



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
  // INSIDE A WALL.  If it is, then
  // Pacman can't make that move,
  // and we'll actually just leave him
  // where he is.
  
  Tile *tile = game->getTileAt( newProposedPos ) ;
  if( tile->isPassable() )
  {
    // Allow the move
    pos += vel ;   // move him
  }
  else
  {
    // Disallow the move
  }

  // snap him to his tile space.
  squareOff() ;

  
}


void FourDirectionMovingObject::draw() // override
{
  float x = pos.x + game->getBoardOffsetX() ;
  float y = pos.y + game->getBoardOffsetY() ;
  int ts = game->tileSize ;

  switch( motionState )
  {
  case MotionState::MovingRight:
    // The sprite is assumed to be facing right
    //window->drawSprite( spriteId, 
    window->drawSprite( spriteId,
      x,y,
      ts, ts ) ;
    break ;

  case MotionState::MovingLeft:
    // Flip left.  Not a rotation of 180 degrees
    // because that would make pacman upside down.
    window->drawSprite( spriteId, x,y, -ts, ts ) ;
    break ;

  case MotionState::MovingUp:
    // Rotate 90 degrees CCW
    window->drawSprite( spriteId, x,y,
      ts, ts, -90 ) ;
    break ;

  case MotionState::MovingDown:
    // rotate 90 degrees CW
    window->drawSprite( spriteId, x,y,
      ts, ts, 90 ) ;
    break ;
  }
}



