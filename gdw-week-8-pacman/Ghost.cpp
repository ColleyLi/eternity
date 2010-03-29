#include "Ghost.h"
#include "GameWorld.h"

Ghost::Ghost()
{
  // default speed is 0.5f
  speed = 0.5f ;
}

Ghost::~Ghost()
{

}

void Ghost::enqueueNextMove()
{
  // Randomly choose a new direction to move in
  MotionState nextDir ;

  switch( name )
  {
  case 'i': // (i)nky will move randomly
    nextDir = (MotionState)(MotionState::MovingLeft + rand()%4) ;
    break ;
  case 'p':
    {
      // (p)inky will avoid you
      Vector2 away = pos - game->getPlayer()->pos ;

      // "round" that vector off to a "direction"
      nextDir = getNearestDirection( away ) ;
    }
    break;
  case 's':
    // (s)ue will move towards
    {
      Vector2 towards = game->getPlayer()->pos ;
      nextDir = getNearestDirection( towards ) ;
    }
    break ;

  case 'b':
  default:
    // (b)linky will move sometimes twoards, sometimes away
    // ranodmly (50/50)
    if( rand() % 2 ) // 50% chance of this being (non-zero)
    {
      // avoid you
      Vector2 away = pos - game->getPlayer()->pos ;
      nextDir = getNearestDirection( away ) ;
    }
    else
    {
      // go for him
      Vector2 towards = game->getPlayer()->pos ;
      nextDir = getNearestDirection( towards ) ;
    }
    break ;

  }

  nextMoves.push( nextDir ) ;
}

void Ghost::step( float time ) //override
{
  MotionState nextDir ;
  if( nextMoves.empty() )
    enqueueNextMove() ;    // push a dir
  
  nextDir = nextMoves.front() ;

  // request next direction
  if( reqMotionState( nextDir ) )
  {
    // If this goes through, then it means,
    // this request has been COMMITTED
    // and the ghost is now moving
    // in that newly requested direction.
    // so pop it from the queue.
    nextMoves.pop() ; // won't be empty due
    // to push above
  }
  else
  {
    // He can't go that way.  So pop anyway
    nextMoves.pop();
  }


  //!! i'm thinking perhaps we need to just
  // pop the stack every 60 moves (every second)
  // in case he gets stuck in the wall.. (p)inky will
  // probably get stuck!
  static int last = 0 ;
  last++ ;
  if( last > 10 )
  {
    if( !nextMoves.empty() )
      nextMoves.pop() ;
    last = 0 ;
  }




  // now invoke the base-clsas stepping mechanism,
  // which gets you grid-aligned movement
  FourDirectionMovingObject::step( time ) ;
}

void Ghost::draw()
{
  float x = pos.x + game->getBoardOffsetX() ;
  float y = pos.y + game->getBoardOffsetY() ;
  int ts = game->tileSize ;


  // Draw the ghost body regardless, this
  // part doesn't change orientation for the
  // ghosts
  // What does change however is the "shading color" applied.
  // (i)nky is blue
  // (p)inky is
  // (b)linky is purple
  // (s)ue is red
  // The body is spriteId, eyes are overlaid on top
  int ghostAlpha = 220 ; // they're ghosts!  partly see-thru
  // all the time!
  D3DCOLOR shadeColor ;
  switch( name )
  {
  case 'i':
    shadeColor = D3DCOLOR_ARGB( ghostAlpha, 96, 160, 255 ) ;
    break;
  case 'p':
    shadeColor = D3DCOLOR_ARGB( ghostAlpha, 255, 176, 220 ) ;
    break;
  case 'b':
    shadeColor = D3DCOLOR_ARGB( ghostAlpha, 192, 176, 255 ) ;
    break;
  case 's':
    shadeColor = D3DCOLOR_ARGB( ghostAlpha, 72, 0, 160 ) ;
    break;

  default:
    // just make it a translucent white,
    // indicating some kind of bug (unrecognized ghost name)
    warning( "%c wasn't a recognized ghost name!", name ) ;
    shadeColor = D3DCOLOR_ARGB( ghostAlpha, 255, 255, 255 ) ;
  }
  window->drawSprite( spriteId, x, y, shadeColor ) ;

  // Now draw the eyes
  switch( requestedMotionState )
  {
  case MotionState::MovingRight:
    window->drawSprite( Sprites::EyesRight, x, y, ts, ts ) ;
    break;
  case MotionState::MovingLeft:
    // flip EyesRight horizontally
    window->drawSprite( Sprites::EyesRight, x, y, -ts, ts ) ;
    break ;
  case MotionState::MovingUp:
    window->drawSprite( Sprites::EyesUp, x, y ) ;
    break;
  case MotionState::MovingDown:
    window->drawSprite( Sprites::EyesDown, x, y ) ;
    break;
  case MotionState::Stopped:
    // stopped ONLY happens when the ghost
    // dies, actually
    window->drawSprite( Sprites::EyesDead, x, y ) ;

  }
}