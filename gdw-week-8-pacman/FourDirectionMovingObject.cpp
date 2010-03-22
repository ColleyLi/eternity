#include "FourDirectionMovingObject.h"
#include "GameWorld.h"

FourDirectionMovingObject::FourDirectionMovingObject()
{
  motionState = MotionState::MovingRight ;
  requestedMotionState = motionState ;
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

  /*
        0   1   2   3   4
      +---+---+---+---+---+
      |   |   |   |###|   |
    0 |   |   |   |###|   |
      |   |   |   |###|   |
      +---+---+---+---+---+
      |   |   | P |###|   |
    1 |   |   |PPP|###|   |
      |   |   | P |###|   |
      +---+---+---+---+---+

  
  Find if moving object "encroaches" on tile to right.
  Looking at the diagram above, pacman's (x,y)
  position is IDEALLY (2.0, 1.0).

  However he starts to "encroach" on the tile
  that has a ##WALL## basically when he has
  an x value like 2.01, 2.1, 2.2, 2.3... etc.
  once he reaches 2.5 he would be considered to be
  in that next wall tile to the right.

  If his X value is 1.87 however, he is still
  in tile (2,1).  So in that case he's actually
  encroaching on the tile to the LEFT.

  Encroaching on tile to right:
  --
  So if the CEILING of the X value is
  NOT equal to the TILE the GameWorld object
  thinks he's in, then he is encroaching on
  the tile to the right.

  If he is encroaching on the tile to
  the right AND that tile is a wall/impassible tile
  THEN HE MUST BE PUSHED BACK OFF IT INTO
  HIS ORIGINAL OWN ZONE.

  */

  

  // sitTile is the tile that
  // this object is truly considered
  // to be sitting in.
  float ts = game->tileSize ;

  Tile *sitTile = game->getTileAt( pos ) ;

  // This is where the pacman is SUPPOSED to be
  // if he were actually sitting in the center
  // of his tile..
  float sitX = (round( pos.x / ts ))*ts ;
  float sitY = (round( pos.y / ts ))*ts ;

  //info( "(%.2f, %.2f) => (%.2f, %.2f)", pos.x, pos.y, sitX, sitY ) ;

  Tile *nearestRight = game->getTileNearestRight( pos ) ;

  if( sitTile != nearestRight )  // the player is encroaching on a tile to the RIGHT of him
  {
    if( !nearestRight->isPassable() )   // that tile the player is encroaching up on IS NOT passable
    {
      // Then LOCK HIM TO SIT IN HIS sitTile
      // with respect to X
      pos.x = sitX ;
    }
  }

  Tile *nearestLeft = game->getTileNearestLeft( pos ) ;
  if( sitTile != nearestLeft ) 
    if( !nearestLeft->isPassable() )
      pos.x = sitX ;
      
  Tile *nearestDown = game->getTileNearestDown( pos ) ;
  if( sitTile != nearestDown )
    if( !nearestDown->isPassable() )
      pos.y = sitY ;

  Tile *nearestUp = game->getTileNearestUp( pos ) ;
  if( sitTile != nearestUp )  // encroaching upward
    if( !nearestUp->isPassable() )
      pos.y = sitY ;

}

void FourDirectionMovingObject::reqMotionState( MotionState newMotionState )
{
  requestedMotionState = newMotionState ;

  // first of all to make any direction change
  // he must be already nicely aligned on a tile.
  // if not, refuse the request now

  // Nice tile alignment means
  // that his coordinates are
  // __nearly__ a multiple of 16.
  #define NEARLY_DIVISIBLE 0.1f
  if( (
      fmodf( pos.x, 16.0f ) < NEARLY_DIVISIBLE || // its near from above
      fmodf( pos.x + 2*NEARLY_DIVISIBLE, 16.0f ) < NEARLY_DIVISIBLE // its near from below
      )
      &&
      (
      fmodf( pos.y, 16.0f ) < NEARLY_DIVISIBLE || // its near from above
      fmodf( pos.y + 2*NEARLY_DIVISIBLE, 16.0f ) < NEARLY_DIVISIBLE // its near from below
      )
    )
  {
  }
  else
  {
    // NOT ALIGNED, so motion isn't allowed yet
    return ;
  }

  
  Tile *adjTile ;
  float ts = game->tileSize;

  // Check if its possible to go that way now.
  switch( requestedMotionState )
  {
  case MotionState::MovingUp:
    adjTile = game->getTileNearestUp( pos + Vector2(0,-ts) ) ;
    break ;
  case MotionState::MovingDown:
    adjTile = game->getTileNearestDown( pos + Vector2(0,ts) ) ;
    break;
  case MotionState::MovingLeft:
    adjTile = game->getTileNearestLeft( pos + Vector2(-ts,0) ) ;
    break;
  case MotionState::MovingRight:
    adjTile = game->getTileNearestRight( pos + Vector2(ts,0) ) ;
    break;

  case MotionState::Stopped:
    // Nothing to check here.
    // you can always stop
    motionState = newMotionState ;
    return ;
  default:
    warning( "reqMotionState: You didn't specify a valid direction" ) ;
    adjTile = game->getTileAt( pos ) ; // should not happen.. just get tile already on.
    break;
  }

  if( adjTile->isPassable() )
  {
    // Just set the motion state then
    motionState = newMotionState ;
    //info( "Making the direction change" ) ;
  }
  else
  {
    //warning( "You can't go that way" ) ;
  }
}

void FourDirectionMovingObject::step( float time ) // override
{
  // First re-request any pending change
  // to motion state
  if( requestedMotionState != motionState )
    reqMotionState( requestedMotionState ) ;

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

void FourDirectionMovingObject::intersects( Tile *tile )
{
  
}

/// What to do when this FourDirectionMovingObject
/// is intersected by another GameObject
void FourDirectionMovingObject::intersects( GameObject *other )
{
}

/// What to do when this FourDirectionMovingObject
/// is intersected by another FourDirectionMovingObject
void FourDirectionMovingObject::intersects( FourDirectionMovingObject *other )
{
}


