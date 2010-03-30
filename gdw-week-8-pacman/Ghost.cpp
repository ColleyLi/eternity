#include "Ghost.h"
#include "GameWorld.h"

Ghost::Ghost()
{
  speed = 1.0f ;

  // Wait 60 frames before releasing
  // the ghosts from the vault
  framesSinceLastRecomputeMoves = FramesToWaitBetweenPathComputes - 60 ;
}

Ghost::~Ghost()
{
  // We don't delete this->nextMoves, 
  // since it actually belongs to the
  // pathfinder
}

void Ghost::setName( char newName )
{
  name = newName ;

  // (i)nky is blue
  // (p)inky is
  // (b)linky is purple
  // (s)ue is red
  // The body is spriteId, eyes are overlaid on top
  int ghostAlpha = 180 ; // they're ghosts!  partly see-thru
  // all the time!

  switch( name )
  {
  case 'i':
    shadingColor = D3DCOLOR_ARGB( ghostAlpha, 96, 160, 255 ) ;
    break;
  case 'p':
    shadingColor = D3DCOLOR_ARGB( ghostAlpha, 255, 176, 220 ) ;
    break;
  case 'b':
    shadingColor = D3DCOLOR_ARGB( ghostAlpha, 192, 176, 255 ) ;
    break;
  case 's':
    shadingColor = D3DCOLOR_ARGB( ghostAlpha, 128, 0, 0 ) ;
    break;

  default:
    // just make it a translucent white,
    // indicating some kind of bug (unrecognized ghost name)
    warning( "%c wasn't a recognized ghost name!", name ) ;
    shadingColor = D3DCOLOR_ARGB( ghostAlpha, 255, 255, 255 ) ;
  }

}

void Ghost::pathfindTowardsRandomPosition()
{
  // Get the tile that corresponds
  // to where the ghost is now
  Coord currentPos = game->getNearestCoord( pos ) ;

  // Choose a tile on the map
  // that is reachable
  goalNode = game->getAsciiMap()->getRandomWalkableTile() ;

  // generate the path
  nextMoves = game->getAsciiMap()->solve( currentPos, goalNode ) ;

  framesSinceLastRecomputeMoves = 0 ;
}

void Ghost::step( float time ) //override
{
  // Ok, here's the thing.  We need
  // these ghosts to move around intelligently,
  // correct?  Yes, we do.

  // BUT we don't want the ghosts ALL make a 
  // beeline for the player using the shortest path
  // every time.  That isn't fair and would make
  // the game wayy too hard.

  // What we'll do instead is
  // this:
  //   1.  Every 90 frames or so (second and a half)
  //       have ghost CHOOSE A RANDOM SPOT ON THE MAP
  //       TO GO TO.  This will give the ghosts a 
  //       WANDERING / EXPLORATORY thing.
  //
  //   2.  Do an A* pathfind TO THAT RANDOM SPOT SELECTED,
  //       which'll move the ghost through the maze, without
  //       too much silly <- -> (back and forth motion
  //       on the same 2 tiles).
  //
  //   3.  OCCASSIONALLY do an A* pathfind to the EXACT PLAYER
  //       LOCATION, causing ghost to try and "go for" the player.
  //       We'll purposefully NOT do this every frame,
  //       so that if the player moves, the ghost will end up
  //       in an "old" location of the player, which is fine,
  //       we don't want the ghosts to be too aggressive anyway.

  framesSinceLastRecomputeMoves++ ;
  if( framesSinceLastRecomputeMoves > FramesToWaitBetweenPathComputes )
  {
    // recompute moves
    pathfindTowardsRandomPosition() ;
  }

  // Now, you can only change the move
  // when you reach "square alignment"
  if( isSquare( game->tileSize ) )
  {
    if( !nextMoves.empty() )
    {
      Coord next = nextMoves.front() ;

      Vector2 vecNextTile = game->getVectorAt( next ) ;

      // Find the vector that will get me towards that tile
      Vector2 vecTowardsNextTile = vecNextTile - pos ;

      // now we can find what the next motion state
      // this guy should have:
      MotionState nextMotionState = getNearestDirection( vecTowardsNextTile ) ;

      // and we REQUEST it as the next motion state,
      // so the ghost will try and move in that direction
      reqMotionState( nextMotionState ) ;

      // Now pop off the front, we've used it
      nextMoves.pop_front() ;
    }
    else
    {
      // next moves are empty!
      //info( "Ghost ran out of moves!" ) ;

      // out of moves early, so just
      // regenerate
      pathfindTowardsRandomPosition() ; 
    }
  }

  // Extensions:
  //   Not done here, but it would be rather
  //   cool if we did the following:
  //     1.  If SUE (red) has "line-of-sight"
  //         with the player (i.e. if in same
  //         row or same column with no obstacles between)
  //         Then SUE does a "perfect" A* pathfind
  //         towards the player (vigorous pursuit of the player).
  //         I'll do that later though.

  // now invoke the base-class stepping mechanism,
  // which gets you grid-aligned movement
  FourDirectionMovingObject::step( time ) ;
}

void Ghost::draw()
{
  Vector2 drawVec = game->getDrawingVectorAt( pos ) ;
  int ts = game->tileSize ;

  #pragma region draw the ghost
  // Draw the ghost body.
  // Same sprite used for all orientations.
  // Apply his shading color to him
  // making the different ghosts unique
  window->drawSprite( spriteId, drawVec.x, drawVec.y, shadingColor ) ;

  // Now draw the eyes.  Depends
  // on direction ghost will go __next__.
  switch( requestedMotionState )
  {
  case MotionState::MovingRight:
    window->drawSprite( Sprites::EyesRight, drawVec.x, drawVec.y, ts, ts ) ;
    break;
  case MotionState::MovingLeft:
    // flip EyesRight horizontally
    window->drawSprite( Sprites::EyesRight, drawVec.x, drawVec.y, -ts, ts ) ;
    break ;
  case MotionState::MovingUp:
    window->drawSprite( Sprites::EyesUp, drawVec.x, drawVec.y ) ;
    break;
  case MotionState::MovingDown:
    window->drawSprite( Sprites::EyesDown, drawVec.x, drawVec.y ) ;
    break;
  case MotionState::Stopped:
    // stopped ONLY happens when the ghost
    // dies, actually.  So we use the
    // Stopped state to indicate the
    // ghost should be drawn with "dead eyes" (xx)
    window->drawSprite( Sprites::EyesDead, drawVec.x, drawVec.y ) ;

  }

  #pragma endregion
  
  // For debug, draw his goal node
  //Vector2 goalNodeVec = game->getDrawingVectorAt( goalNode ) ;
  //window->drawSprite( Sprites::GoalNode, goalNodeVec.x, goalNodeVec.y, shadingColor ) ;
  
}